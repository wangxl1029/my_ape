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
#include <set>
#include <unordered_set>
#include "NeurConscious.hpp"
#include "NeuronDef.hpp"


namespace nsAI {
    namespace nsNeuronal{

        class CEmoCached : public CObject
        {
        public:
			CEmoCached() = default;
			CEmoCached(std::shared_ptr<CNeuron> spNeur);
            ~CEmoCached() final = default;
			void strengthen();
			std::shared_ptr<CNeuron> getNeuron();
		private:
			std::shared_ptr<CNeuron> m_spNeuron;
        };
        
        class CThink::CPrivate : public CObject
        {
        public:
			CPrivate();
            ~CPrivate() final = default;
			void tense(std::unique_ptr<CEmotion>);
		private:

			std::set<size_t> m_tagUndupped;
			std::vector<size_t> m_tagSeq;
			std::shared_ptr<CTagIndex> m_tagIndex;
			std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
			std::shared_ptr<CNeuronPool> m_spNeurPool;
        };
        
		CThink::CPrivate::CPrivate() 
			: m_tagIndex(std::make_shared<CTagIndex>())
			, m_spNeurPool(std::make_shared<CNeuronPool>())
		{
		}

		void CThink::CPrivate::tense(std::unique_ptr<CEmotion> e)
		{
			std::cout << "mind: tense " << CEmotion::echo(e->m_tag) << std::endl;

			auto curNeur = m_spNeurPool->buildNeuron(e->m_tag);
			curNeur->strengthen();
			m_vecUndupNeuron.push_back(curNeur);

			bool ok, isIdxOK = false;
			std::tie(std::ignore, ok) = m_tagUndupped.insert(e->m_tag);
			if ( ! ok)
			{
				auto spIdx = std::make_shared<CTagIndex::TagVec_t>();
				spIdx->swap(m_tagSeq);
				isIdxOK = m_tagIndex->Insert(spIdx);
				if (isIdxOK && spIdx->size() > 1)
				{
					auto newNeur = m_spNeurPool->buildNeuron(CEmotion::getUniqueTag());
					auto spDendrite = newNeur->buildDendrite();
					std::for_each(m_vecUndupNeuron.begin(), m_vecUndupNeuron.end(),
						[spDendrite](std::shared_ptr<CNeuron> spN) {spDendrite->attach(spN->buildAxon()); });
					m_vecUndupNeuron.clear();
					m_tagUndupped.clear();
				}
			}

			m_tagSeq.push_back(e->m_tag);
		}

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

		void CThink::initialize(CBusClient *pCortex, CEmotionTarget *pUnconsci, CEmotionTarget *pConsci)
		{
			assert(pCortex);
			assert(pUnconsci);
			assert(pConsci);

			m_pCortex = pCortex;
			m_pUnconsci = pUnconsci;
			m_pConscious = pConsci;
		}

		CEmoCached::CEmoCached(std::shared_ptr<CNeuron> spNeur) : m_spNeuron(spNeur)
		{
		}

		inline void CEmoCached::strengthen()
		{ 
			assert(m_spNeuron);
			m_spNeuron->strengthen();
		}

		std::shared_ptr<CNeuron> CEmoCached::getNeuron()
		{
			return m_spNeuron;
		}

	}
}

void nsAI::nsNeuronal::CThink::operator()()
{
    while (m_pCortex->isAlive())
    {
        auto e = m_pConscious->getEmotion();
        if (e)
        {
			mp-> tense(std::move(e));
        }
        else
        {
            std::cout << "mind: idle" << std::endl;
        }
    }
}

