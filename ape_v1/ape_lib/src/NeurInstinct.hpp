//
//  ape_instinct.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_instinct_hpp
#define ape_instinct_hpp

namespace nsAI {
    namespace nsNeuronal{
        class CLayer;
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
    }
}
#endif /* ape_instinct_hpp */