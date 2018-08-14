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
