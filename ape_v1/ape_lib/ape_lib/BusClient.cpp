//
//  BusClient.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#include <cassert>
#include "BusClient.hpp"

namespace ns_ = nsAI::nsNeuronal;

ns_::CBusClient::CBusClient() : m_isAlive(true)
{
}

void ns_::CBusClient::connect(IBusServer* bus, IBusServer::CConnectiveTarget_E target)
{
    assert(bus);
    bus->Connect(target, this);
    m_bus = bus;
}

bool ns_::CBusClient::isAlive() const
{
    return m_isAlive.load();
}

