//
//  NeuronalLayerPool.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/15.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef NeuronalLayerPool_hpp
#define NeuronalLayerPool_hpp

namespace nsAI {
    namespace nsNeuronal
    {
        class CLayerPool : public CObject
        {
            class CPrivate;
        public:
            CLayerPool();
            void Send(std::unique_ptr<CEmotion>);
            void Kill();

			CProperty_TSRW<std::function<void(std::unique_ptr<CEmotion>)>> m_propSendingEmotion;
        private:
            std::shared_ptr<CPrivate> mp;
        };
    }
}

#endif /* NeuronalLayerPool_hpp */
