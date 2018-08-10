//
//  ape_mind.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_mind_hpp
#define ape_mind_hpp

#include "BusClient.hpp"
#include "EmotionTarget.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class CThink
        {
            class CPrivate;
        public:
            CThink();
            ~CThink() = default;
            
            void initialize(CBusClient* pCortex, CEmotionTarget* pUnconsci, CEmotionTarget* pConsci);
            void operator()();
        private:
            
            CBusClient* m_pCortex;
            CEmotionTarget* m_pUnconsci;
            CEmotionTarget* m_pConscious;
            std::shared_ptr<CPrivate> mp;
        };
        
        class CConscious : public CEmotionTarget
        {
        public:
            CConscious() = default;
            ~CConscious() final = default;
            
            CConscious& operator=(std::thread&&);
        };
    }
}

#endif /* ape_mind_hpp */
