//
//  ape_instinct.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>

#include "ai_comm.hpp"
#include "EmotionTarget.hpp"
#include "BusClient.hpp"
#include "NeurInstinct.hpp"

using namespace nsAI::nsNeuronal;

CInstinct::CInstinct()
: m_pOwner(nullptr)
, m_pUnconsci(nullptr)
, m_isSleeping(false)
, m_isTired(false)
, m_nHungery(0)
, m_nCrying(0)
{
}

void CInstinct::Initialize(nsAI::nsNeuronal::CBusClient *owner, nsAI::nsNeuronal::CEmotionTarget *sensor)
{
    m_pOwner = owner;
    m_pUnconsci = sensor;
}

void CInstinct::operator()()
{
    assert(m_pOwner);
    //    auto spNL = std::make_shared<CLayer>();
    
    while (m_pOwner->isAlive())
    {
        bool isEnableWaken = true;
        auto msg = m_pOwner->getmsg();
        if (msg)
        {
            switch (msg->m_ID)
            {
                case nsBus::CMessageId_E::CORTEX_TEST:
                    if (!m_isSleeping)
                    {
                        m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::input_test));
                    }
                    break;
                    
                case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
                    if (!m_isSleeping)
                    {
                        m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::input_absence));
                        if (m_nHungery++ <= HANGERY_MAX)
                        {
                            m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_crying));
                        }
                        
                        if (m_nCrying++ <= CRYING_MAX)
                        {
                        }
                        else
                        {
                            m_isSleeping = m_isTired = true;
                            m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_sleep));
                        }
                    }
                    else
                    {
                        isEnableWaken = false;
                    }
                    break;
                    
                case nsBus::CMessageId_E::CORTEX_TEXT_INPUT:
                    if (!m_isSleeping)
                    {
                        m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::input_txt));
                    }
                    break;
                    
                default:
                    break;
            }
            
            
        }
        else
        {
            //std::cout << "instinct idle" << std::endl;
            m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_idle));
        }
        
        if (m_isSleeping && m_isTired)
        {
            relex();
        }
        
        if (m_isSleeping && !m_isTired && isEnableWaken)
        {
            m_isSleeping = false;
        }
    }
}

void CInstinct::relex()
{
    if (m_nHungery > 0)
    {
        if (0 == m_nHungery--)
            m_isTired = false;
    }
    
    if (m_nCrying > 0)
    {
        m_nCrying = 0;
    }
}


