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
            CEmotion(CEmotion_E t) : m_tag(static_cast<size_t>(t))
            {
                
            }
            
            ~CEmotion() override
            {
            }
            
            static std::string echo(size_t tagval)
            {
                std::string s = "tag unkown";
                if (tagval < EMOTION_E_MAX)
                {
                    switch (static_cast<CEmotion_E>(tagval))
                    {
                        case CEmotion_E::input_absence:
                            s = "input absense";
                            break;
                        case CEmotion_E::input_txt:
                            s = "input text";
                            break;
                        case CEmotion_E::instinct_sleep:
                            s = "instinct sleep";
                            break;
                        case CEmotion_E::instinct_crying:
                            s =  "instinct crying";
                            break;
                        case CEmotion_E::instinct_idle:
                            s = "instinct idle";
                            break;
                        default:
                            break;
                    }
                }
                
                return s;
            }

			static size_t getUniqueTag()
			{
				static std::atomic_size_t tag = EMOTION_E_MAX;
				return tag++;
			}
            
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
