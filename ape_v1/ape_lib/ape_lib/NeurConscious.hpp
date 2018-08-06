//
//  ape_mind.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
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
            bool isCached(size_t);
            bool isCached(size_t, size_t);
            size_t getPreEmotion();
            void tense(std::unique_ptr<CEmotion>);
            size_t buildCached();
            
            std::vector<size_t> m_vecCahcedIdx;
            CBusClient* m_pCortex;
            CEmotionTarget* m_pUnconsci;
            CEmotionTarget* m_pConscious;
            std::shared_ptr<CPrivate> mp;
        };
        
        class CMind : public CEmotionTarget
        {
        public:
            CMind() = default;
            ~CMind() final = default;
            
            void ComeToSense(CBusClient* parent)
            {
                m_thread = std::thread(m_think);
            }
            
        private:
            CThink m_think;
        };
    }
}


#endif /* ape_mind_hpp */
