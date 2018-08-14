//
//  EmotionTarget.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef EmotionTarget_hpp
#define EmotionTarget_hpp

#include "ai_active.hpp"
#include "NeurEmotion.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class CEmotionTarget : public  CActiveTarget<CEmotion>
        {
        public:
            CEmotionTarget()
            {
            }
            
            ~CEmotionTarget() override
            {
            }
            
            void Send(std::unique_ptr<CEmotion> e)
            {
                PushMessage(std::move(e));
            }
            
            std::unique_ptr<CEmotion> getEmotion()
            {
                return PopMessage();
            }
            
            void Kill() final
            {
                // can not interfer my sycho
            }
        private:
            
        };

    }
}

#endif /* EmotionTarget_hpp */
