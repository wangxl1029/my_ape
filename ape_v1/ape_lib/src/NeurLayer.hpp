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
        class CLayerLifeCycle : public ILifeCycle
        {
        public:
            CLayerLifeCycle();
            bool isAlive() final;
            void Reset(bool);
        private:
            std::atomic_bool m_alive;
        };
        
        class CLayer : public CEmotionTarget
        {
            class CPrivate;
        public:
            CLayer();
            ~CLayer() override = default;
        private:
            std::shared_ptr<CPrivate> mp;
        };
        
        class CLayerProxy : public CObject
        {
        public:
            CLayerProxy();
            ~CLayerProxy() final = default;
            void Send_TS(std::unique_ptr<CEmotion>);
        private:
            std::shared_ptr<CLayer> m_spProxy;
			CLayer* m_pLayer;
        };

		class CLayerWork
		{
		public:
			CLayerWork(ILifeCycle&);
			void operator()();
		private:
			ILifeCycle & m_lc;
			std::shared_ptr<CLayerProxy> m_spProxy;
		};
	}
}

#endif /* neur_priv_hpp */
