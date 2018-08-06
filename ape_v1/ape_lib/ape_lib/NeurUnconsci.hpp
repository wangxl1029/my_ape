//
//  NeurUnconsci.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef NeurUnconsci_hpp
#define NeurUnconsci_hpp

#include "EmotionTarget.hpp"
#include "BusClient.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class CUnconscious: public CEmotionTarget
        {
        public:
            CUnconscious() = default;
            ~CUnconscious() final = default;
            
            CUnconscious& operator=(std::thread&& t)
            {
                m_thread = std::move(t);
                return *this;
            }
        };
        
        class CReflect : public CObject
        {
        public:
            CReflect() : m_pCortex(nullptr), m_pUnconsci(nullptr)
            {
            }
            ~CReflect() final = default;
            
            void Intialize(CBusClient* cortex, CEmotionTarget* sensor, CEmotionTarget* mind)
            {
                m_pCortex = cortex;
                m_pMind = mind;
                m_pUnconsci = sensor;
            }
            
            void operator()();
            
        private:
            std::unique_ptr<CEmotion> triggerReflexUnconsciously(std::unique_ptr<CEmotion> e)
            {
                return e;
            }
            
            CBusClient * m_pCortex;
            CEmotionTarget* m_pUnconsci;
            CEmotionTarget* m_pMind;
        };
        
    }
}




#endif /* NeurUnconsci_hpp */
