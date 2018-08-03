//
//  BusServer.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusServer_h
#define BusServer_h
#include "ai_commPriv.hpp"
#include "BusMessage.hpp"

namespace nsAI {
    namespace nsNeuronal{
        namespace nsBus{
            class IBusServer
            {
            public:
                enum class CConnectiveTarget_E
                {
                    cortex, input, output
                };
                virtual void Send(std::unique_ptr<nsBus::CMessage> m) = 0;
                virtual void Connect(CConnectiveTarget_E type, CActiveTarget<nsBus::CMessage>* target) = 0;
                virtual ~IBusServer() = default;
            };
            
        }
    }
}


#endif /* BusServer_h */
