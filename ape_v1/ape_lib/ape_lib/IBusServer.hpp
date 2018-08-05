//
//  BusServer.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusServer_h
#define BusServer_h
#include "BusMsgTarget.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class IBusServer
        {
        public:
            enum class CConnectiveTarget_E
            {
                cortex, input, output
            };
            virtual void Send(std::unique_ptr<nsBus::CMessage> m) = 0;
            virtual void Connect(CConnectiveTarget_E type, nsBus::CMessageTarget_t* target) = 0;
            virtual ~IBusServer() = default;
        };
    }
}


#endif /* BusServer_h */
