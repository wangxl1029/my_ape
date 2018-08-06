//
//  ape_mind.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>
#include "NeurConscious.hpp"

namespace ns_ = nsAI::nsNeuronal;

ns_::CThink::CThink()
: m_pCortex(nullptr)
, m_pUnconsci(nullptr)
, m_pConscious(nullptr)
{}

void ns_::CThink::initialize(ns_::CBusClient *pCortex, ns_::CEmotionTarget *pUnconsci, ns_::CEmotionTarget *pConsci)
{
    assert(pCortex);
    assert(pUnconsci);
    assert(pConsci);
    
    m_pCortex = pCortex;
    m_pUnconsci = pUnconsci;
    m_pConscious = pConsci;
}

void ns_::CThink::operator()()
{
    while (m_pCortex->isAlive())
    {
        auto e = m_pConscious->getEmotion();
        if (e)
        {
            std::cout << "mind: " << CEmotion::echo(e->m_tag) << std::endl;
            
        }
    }
}
