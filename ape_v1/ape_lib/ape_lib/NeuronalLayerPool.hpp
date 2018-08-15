//
//  NeuronalLayerPool.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/15.
//  Copyright © 2018年 alan king. All rights reserved.
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
        private:
            std::shared_ptr<CPrivate> mp;
        };
    }
}

#endif /* NeuronalLayerPool_hpp */
