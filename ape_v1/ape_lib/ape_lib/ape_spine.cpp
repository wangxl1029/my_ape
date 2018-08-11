//
//  ape_spine.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <iostream>

#include "ape.hpp"
#include "BusMsgTarget.hpp"
#include "IBusServer.hpp"
#include "ape_spine.hpp"

namespace ns_ = nsAI::nsNeuronal;

void nsAI::CApe::CSpine::Send(std::unique_ptr<ns_::nsBus::CMessage> m)
{
	ns_::nsBus::CMessageTarget_t* target = nullptr;
	if (m->m_ID >= ns_::nsBus::CMessageId_E::CORTEX_TEST && m->m_ID < ns_::nsBus::CMessageId_E::CORTEX_ERROR)
	{
		target = m_pCortex.load();
	}
	else if (m->m_ID >= ns_::nsBus::CMessageId_E::INPUT_TEST && m->m_ID < ns_::nsBus::CMessageId_E::INPUT_ERROR)
	{
		target = m_pInput.load();
	}

	if (target)
	{
		target->PushMessage(std::move(m));
	}
}

void nsAI::CApe::CSpine::Connect(ns_::IBusServer::CConnectiveTarget_E type, ns_::nsBus::CMessageTarget_t* target)
{
	switch (type)
	{
	case ns_::IBusServer::CConnectiveTarget_E::cortex:
		m_pCortex.store(target);
		break;
	case ns_::IBusServer::CConnectiveTarget_E::input:
		m_pInput.store(target);
		break;
	case ns_::IBusServer::CConnectiveTarget_E::output:
		m_pOutput.store(target);
		break;
	default:
		std::cerr << "Unknown target" << std::endl;
		break;
	}
}
