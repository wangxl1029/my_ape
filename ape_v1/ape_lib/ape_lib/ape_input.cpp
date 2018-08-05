//
//  ape_input.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//

#include <iostream>
#include "ape_input.hpp"
#include "ape_spine.hpp"

nsAI::CApe::CInput::CInput(std::shared_ptr<CSpine> spSpine)
{
    auto pBusServer = dynamic_cast<nsNeuronal::IBusServer*>(spSpine.get());
    connect(pBusServer, nsNeuronal::IBusServer::CConnectiveTarget_E::input);
}

void nsAI::CApe::CInput::process(CBusClient* owner)
{
    auto &Bus = owner->bus();
    while (owner->isAlive())
    {
        auto msg = owner->getmsg();
        if (msg)
        {
            switch (msg->m_ID)
            {
                case nsNeuronal::nsBus::CMessageId_E::INPUT_TEST:
                    std::cout << "ape input test" << std::endl;
                    break;
                default:
                    break;
            }
        }
        else
        {
            Bus.Send(nsNeuronal::nsBus::CMsgIdleInput::CreateUniquePtr());
        }
    }
}
