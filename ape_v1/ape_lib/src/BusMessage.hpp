//
//  BusMessage.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef BusMessage_h
#define BusMessage_h

#include <memory>
#include <string>

#include "ai_comm.hpp"

namespace nsAI {
    namespace nsNeuronal{
        namespace nsBus{
            enum class CMessageId_E
            {
                CORTEX_TEST,
                CORTEX_TEXT_INPUT,
                CORTEX_IDLE_INPUT,
                CORTEX_ERROR,
                INPUT_TEST,
                INPUT_ERROR,
                NONE
            };
            
            class CMessage : public CObject
            {
            public:
                CMessage() : CMessage(CMessageId_E::NONE) {}
                CMessage(CMessageId_E id) : m_ID(id)
                {
                }
                
                ~CMessage() override = default;
                
                const CMessageId_E m_ID;
                std::unique_ptr<CObject> m_upData;
            };

            class CMsgIdleInput : public CMessage
            {
            public:
                CMsgIdleInput() : CMessage(CMessageId_E::CORTEX_IDLE_INPUT)
                {
                }
                
                ~CMsgIdleInput()
                {
                }
                
                static std::unique_ptr<CMsgIdleInput> CreateUniquePtr()
                {
                    return std::make_unique<CMsgIdleInput>();
                }
            };
            
        } ///< nsBus
    } ///< nsNeuronal
} ///< nsAI
#endif /* BusMessage_h */
