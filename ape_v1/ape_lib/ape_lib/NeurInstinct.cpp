//
//  ape_instinct.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//
#include <cassert>
#include "NeurInstinct.hpp"

namespace ns_ = nsAI::nsNeuronal;

void ns_::CInstinct::operator()()
{
    assert(m_pOwner);
    
    while (m_pOwner->isAlive())
    {
        bool isEnableWaken = true;
        auto msg = m_pOwner->getmsg();
        if (msg)
        {
            switch (msg->m_ID)
            {
                case nsBus::CMessageId_E::CORTEX_TEST:
                    if ( ! m_isSleeping)
                    {
                        m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_test));
                    }
                    break;
                    
                case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
                    if ( ! m_isSleeping)
                    {
                        m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_absence));
                        if (m_nHungery++ <= HANGERY_MAX)
                        {
                            m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_crying));
                        }
                        
                        if (m_nCrying++ <= CRYING_MAX)
                        {
                        }
                        else
                        {
                            m_isSleeping = m_isTired = true;
                            m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_sleep));
                        }
                    }
                    else
                    {
                        isEnableWaken = false;
                    }
                    break;
                    
                case nsBus::CMessageId_E::CORTEX_TEXT_INPUT:
                    if ( ! m_isSleeping)
                    {
                        m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_txt));
                    }
                    break;
                    
                default:
                    break;
            }
            
            
        }
        else
        {
            //std::cout << "instinct idle" << std::endl;
            m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_idle));
        }
        
        if (m_isSleeping && m_isTired)
        {
            relex();
        }
        
        if (m_isSleeping && ! m_isTired && isEnableWaken)
        {
            m_isSleeping = false;
        }
    }
}

