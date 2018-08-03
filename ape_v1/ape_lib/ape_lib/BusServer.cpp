//
//  BusServer.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#include <iostream>
#include "BusServer.hpp"

namespace ns = nsAI::nsNeuronal::nsBus;

void ns::CBusServer::Send(std::unique_ptr<nsBus::CMessage> m)
{
    CActiveTarget* target = nullptr;
    if ( m->m_ID >= nsBus::CMessageId_E::CORTEX_TEST && m->m_ID < nsBus::CMessageId_E::CORTEX_ERROR )
    {
        target = m_pCortex.load();
    }
    else if (m->m_ID >= nsBus::CMessageId_E::INPUT_TEST && m->m_ID < nsBus::CMessageId_E::INPUT_ERROR)
    {
        target = m_pInput.load();
    }
    
    if (target)
    {
        target->PushMessage(std::move(m));
    }
}

void ns::CBusServer::Connect(CConnectiveTarget_E type, CActiveTarget* target)
{
    switch (type)
    {
        case IBusServer::CConnectiveTarget_E::cortex:
            m_pCortex.store( target );
            break;
        case IBusServer::CConnectiveTarget_E::input:
            m_pInput.store( target );
            break;
        case IBusServer::CConnectiveTarget_E::output:
            m_pOutput.store(target);
            break;
        default:
            std::cerr << "Unknown target" << std::endl;
            break;
    }
}
