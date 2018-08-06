//
//  BusProxy.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusProxy_hpp
#define BusProxy_hpp
#include <atomic>
#include "IBusServer.hpp"

namespace nsAI {
    namespace nsNeuronal{
        class CBusProxy : public CNoCopyable
        {
        public:
            CBusProxy() : m_pBusServer(nullptr)
            {
            }
            
            ~CBusProxy() final = default;
           
            CBusProxy& operator=(IBusServer* right)
            {
                m_pBusServer.store(right);
                return *this;
            }
            
            void Send(std::unique_ptr<nsBus::CMessage> m);
        private:
            std::atomic<IBusServer*> m_pBusServer;
        };
        

    }
};

#endif /* BusProxy_hpp */
