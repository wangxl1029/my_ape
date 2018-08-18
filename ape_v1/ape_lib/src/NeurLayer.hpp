//
//  neur_priv.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef neur_layer_hpp
#define neur_layer_hpp

namespace nsAI {
    namespace nsNeuronal
    {
        class CLayer : public CEmotionTarget
        {
            class CPrivate;
        public:
            CLayer(size_t tag);
            CLayer(std::thread&&, size_t tag);
            CLayer& operator=(std::thread&&);
            ~CLayer() override = default;
            const size_t m_tag;
        private:
            std::shared_ptr<CPrivate> mp;
        };
        
        class CLayerGenerator;
        class CLayerWork : public CObject
		{
            class CPrivate;
		public:
			CLayerWork(ILifeCycle&, CLayer& owner, CLayerGenerator&, std::function<void(std::unique_ptr<CEmotion>)>);
			void operator()();
		private:
            std::shared_ptr<CPrivate> mp;
		};
        
        class CLayerGenerator : public CNoCopyable
        {
		public:
            CLayer* getNewLayer(ILifeCycle& lc, std::function<void(std::unique_ptr<CEmotion>)>);
        private:
            std::mutex m_mutex;
            std::vector< std::shared_ptr< CLayer > > m_vecLayers;
            std::vector< std::shared_ptr< CLayerWork > > m_vecWork;
        };
	}
}

#endif /* neur_priv_hpp */
