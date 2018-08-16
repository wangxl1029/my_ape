//
//  ape_emotion.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_emotion_hpp
#define ape_emotion_hpp

#include <string>
#include "ai_comm.hpp"

namespace nsAI{
    namespace nsNeuronal{
        enum class CEmotion_E
        {
            input_absence,
            input_test,
            input_txt,
            instinct_crying,
            instinct_sleep,
            instinct_idle,
            max
        };
        
        class CEmotion : public CObject
        {
        public:
            static const size_t EMOTION_E_MAX = static_cast<size_t>(CEmotion_E::max);
			CEmotion(CEmotion_E t);
            explicit CEmotion(size_t t);
 			~CEmotion() override = default;
            
			static std::string echo(size_t tagval);
            static size_t getUniqueTag();
            
            bool isNotConditional() const
            {
                return m_tag < EMOTION_E_MAX;
            }
            const size_t m_tag;
        private:
            
        };
        
    }
}

#endif /* ape_emotion_hpp */
