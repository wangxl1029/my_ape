//
//  ape_instinct.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>
#include <unordered_map>

#include "ai_comm.hpp"
#include "EmotionTarget.hpp"
#include "BusClient.hpp"
#include "NeurInstinct.hpp"
#include "BusMsgText.hpp"

using namespace nsAI::nsNeuronal;

class CInstinct::CPrivate : public CNoCopyable
{
public:
    CPrivate();
    ~CPrivate() final = default;
    void Process();
    inline void relex();
    
    size_t              m_nHungery;
    size_t              m_nCrying;
    bool                m_isTired;
    bool                m_isSleeping;
    CBusClient*         m_pOwner;
    CEmotionTarget*     m_pUnconsci;
    
    std::unordered_map<char, size_t> m_charDict;
};

CInstinct::CPrivate::CPrivate()
: m_pOwner(nullptr)
, m_pUnconsci(nullptr)
, m_isSleeping(false)
, m_isTired(false)
, m_nHungery(0)
, m_nCrying(0)
{
}

void CInstinct::CPrivate::Process()
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
                    if (! m_isSleeping)
                    {
                        m_pUnconsci->Send(std::make_unique<CEmotion>(CEmotion_E::input_test));
                    }
                    break;
                    
                case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
                    if (! m_isSleeping)
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
                        auto upData = nsBus::CMsgText::getDataUniquePtr(std::move(msg->m_upData));
                        for (size_t i = 0; nullptr != upData && i < upData->getCStrSize(); ++i) {
                            auto c = upData->getCharAt(i);
                            bool ok;
                            decltype(m_charDict)::iterator it;
                            std::tie(it, ok) = m_charDict.emplace(c, SIZE_T_MAX);
                            auto& tag = it->second;
                            if (ok) {
                                tag = CEmotion::getUniqueTag();
                            }
                            assert(SIZE_T_MAX != tag);
                            m_pUnconsci->Send(std::make_unique<CEmotionText>(tag, c));
                        }
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

CInstinct::CInstinct() : mp(std::make_shared<CPrivate>())
{
}

void CInstinct::Initialize(nsAI::nsNeuronal::CBusClient *owner, nsAI::nsNeuronal::CEmotionTarget *sensor)
{
    mp->m_pOwner = owner;
    mp->m_pUnconsci = sensor;
}

void CInstinct::operator()()
{
    return mp->Process();
}

void CInstinct::CPrivate::relex()
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


