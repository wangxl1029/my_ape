//
//  ape_input.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>
#include "BusClient.hpp"
#include "ape.hpp"
#include "ape_input.hpp"
#include "ape_spine.hpp"
#include "BusMsgText.hpp"

nsAI::CApe::CInput::CInput(std::shared_ptr<CSpine> spSpine)
{
	assert(spSpine);
	connect(spSpine.get(), nsNeuronal::IBusServer::CConnectiveTarget_E::input);
	m_thread = std::thread(process, this);
}

void nsAI::CApe::CInput::read(const char *txt)
{
	bus().Send(nsNeuronal::nsBus::CMsgText::CreateUniquePtr(txt));
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
