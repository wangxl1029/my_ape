//
//  ape_output.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//

#include <cassert>
#include <iostream>
#include "ape_output.hpp"
#include "ape_spine.hpp"

nsAI::CApe::COutput::COutput(std::shared_ptr<CSpine> spSpine)
{
    assert(spSpine);
    connect(spSpine.get(), nsNeuronal::IBusServer::CConnectiveTarget_E::output);
}

void nsAI::CApe::COutput::process(CBusClient* owner)
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
