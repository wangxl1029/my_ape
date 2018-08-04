//
//  ape_mind.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef ape_mind_hpp
#define ape_mind_hpp

#include "EmotionTarget.hpp"
#include "BusClient.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class CMind : public CEmotionTarget
        {
        public:
            CMind() = default;
            ~CMind() final = default;
            
            void ComeToSense(CBusClient* parent)
            {
                m_thread = std::thread(Think, parent, this);
            }
            
        private:
            static void Think(CBusClient* parent, CEmotionTarget* owner);
        };
    }
}


#endif /* ape_mind_hpp */
