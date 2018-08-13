//
//  ape_spine.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//
#ifdef _WIN32
#include <atomic>
#endif // _WIN32
#include <iostream>

#include "BusMsgTarget.hpp"
#include "IBusServer.hpp"
#include "ape.hpp"
#include "ape_spine.hpp"

struct nsAI::CApe::CSpine::CPrivate : public CNoCopyable
{
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pInput;
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pOutput;
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pCortex;
};

namespace ns_ = nsAI::nsNeuronal;

nsAI::CApe::CSpine::CSpine() : mp(std::make_shared<CPrivate>())
{}

void nsAI::CApe::CSpine::Send(std::unique_ptr<ns_::nsBus::CMessage> m)
{
	ns_::nsBus::CMessageTarget_t* target = nullptr;
	if (m->m_ID >= ns_::nsBus::CMessageId_E::CORTEX_TEST && m->m_ID < ns_::nsBus::CMessageId_E::CORTEX_ERROR)
	{
		target = mp->m_pCortex.load();
	}
	else if (m->m_ID >= ns_::nsBus::CMessageId_E::INPUT_TEST && m->m_ID < ns_::nsBus::CMessageId_E::INPUT_ERROR)
	{
		target = mp->m_pInput.load();
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
		mp->m_pCortex.store(target);
		break;
	case ns_::IBusServer::CConnectiveTarget_E::input:
		mp->m_pInput.store(target);
		break;
	case ns_::IBusServer::CConnectiveTarget_E::output:
		mp->m_pOutput.store(target);
		break;
	default:
		std::cerr << "Unknown target" << std::endl;
		break;
	}
}
