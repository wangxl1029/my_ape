//
//  BusClient.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusClient_hpp
#define BusClient_hpp

#include "ai_priv.hpp"
#include "BusProxy.hpp"
namespace nsAI{
    namespace nsNeuronal{
        class CBusClient : protected CActiveTarget<nsBus::CMessage>
        {
        public:
            CBusClient();
            ~CBusClient() override = default;
            
            void connect(IBusServer* bus, IBusServer::CConnectiveTarget_E target);
            
            
            bool isAlive() const;
           
            std::unique_ptr<nsNeuronal::nsBus::CMessage> getmsg()
            {
                return PopMessage();
            }
            
            CBusProxy& bus()
            {
                return m_bus;
            }
            
            void Kill() override
            {
                std::atomic_store(&m_isAlive, false);
            }
        protected:
            
            std::atomic_bool m_isAlive;
            CBusProxy m_bus;
        };

    }
}
#endif /* BusClient_hpp */
