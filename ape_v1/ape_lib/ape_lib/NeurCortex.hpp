//
//  ape_cortex.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_cortex_hpp
#define ape_cortex_hpp

namespace nsAI {
    namespace nsNeuronal {
        class CInstinct : CObject
        {
        public:
            static const size_t HANGERY_MAX = 100;
            static const size_t CRYING_MAX = 100;
            
            CInstinct();
            ~CInstinct() final = default;
            
            void Initialize(CBusClient* owner, CEmotionTarget* sensor);
            void operator()();
            
        private:
            inline void relex();
            size_t              m_nHungery;
            size_t              m_nCrying;
            bool                m_isTired;
            bool                m_isSleeping;
            CBusClient*         m_pOwner;
            CEmotionTarget*     m_pUnconsci;
        };
        
        class CCortex : public CBusClient
        {
        public:
            CCortex()
            {
                m_cfnInstinct.Initialize(this, &m_thUnconscious);
                m_thread = std::thread(m_cfnInstinct);
                
                m_cfnReflect.Intialize(this, &m_thUnconscious, &m_thConscious);
                m_thUnconscious = std::thread(m_cfnReflect);
                
                m_cfnThink.initialize(this, &m_thUnconscious, &m_thUnconscious);
                m_thConscious = std::thread(m_cfnThink);
            }
            
            ~CCortex() final = default;
            
        private:
			// thread
			CUnconscious	m_thUnconscious;
			CConscious		m_thConscious;
			// class of function
            CInstinct		m_cfnInstinct;
            CReflect		m_cfnReflect;
            CThink			m_cfnThink;
        };

    }
}

#endif /* ape_cortex_hpp */
