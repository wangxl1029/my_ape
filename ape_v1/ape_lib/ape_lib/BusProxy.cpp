//
//  BusProxy.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#include "BusProxy.hpp"


void nsAI::nsNeuronal::CBusProxy::Send(std::unique_ptr<nsBus::CMessage> m)
{
    static thread_local auto pBus = m_pBusServer.load();
    
    if (nullptr == pBus)
    {
        pBus = m_pBusServer.load();
    }
    
    if (pBus)
    {
        pBus->Send(std::move(m));
    }
}
