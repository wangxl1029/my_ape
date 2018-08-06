//
//  ape_mind.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>
#include <algorithm>
#include "NeurConscious.hpp"

namespace nsAI {
    namespace nsNeuronal{
        
        class CEmoCached : public CObject
        {
        public:
            ~CEmoCached() final = default;
            void strengthen() {}
        };
        
        class CThink::CPrivate : public CObject
        {
        public:
            ~CPrivate() final = default;
            CEmoCached* buildCached() {return nullptr;};
            void buildAssociated() {}
        };
        
        CConscious &CConscious::operator=(std::thread&& t)
        {
            m_thread = std::move(t);
            return *this;
        }
        
        CThink::CThink()
        : mp(std::make_shared<CPrivate>())
        , m_pCortex(nullptr)
        , m_pUnconsci(nullptr)
        , m_pConscious(nullptr)
        {}
    }
}

namespace ns_ = nsAI::nsNeuronal;

void ns_::CThink::initialize(ns_::CBusClient *pCortex, ns_::CEmotionTarget *pUnconsci, ns_::CEmotionTarget *pConsci)
{
    assert(pCortex);
    assert(pUnconsci);
    assert(pConsci);
    
    m_pCortex		= pCortex;
    m_pUnconsci		= pUnconsci;
    m_pConscious	= pConsci;
}

bool ns_::CThink::isCached(size_t tag)
{
    return false;
}

size_t ns_::CThink::getPreEmotion()
{
	return SIZE_MAX;
}

size_t ns_::CThink::buildCached()
{
    return SIZE_MAX;
}

void ns_::CThink::tense(std::unique_ptr<CEmotion> e)
{
    std::cout << "mind: tense " << CEmotion::echo(e->m_tag) << std::endl;
    CEmoCached* pCached = nullptr;
    if (! isCached(e->m_tag)) {
        pCached = mp->buildCached();
    }

    assert(pCached);
    pCached->strengthen();
    
    mp->buildAssociated();
}

void ns_::CThink::operator()()
{
    while (m_pCortex->isAlive())
    {
        auto e = m_pConscious->getEmotion();
        if (e)
        {
            tense(std::move(e));
        }
        else
        {
            std::cout << "mind: idle" << std::endl;
        }
    }
}
