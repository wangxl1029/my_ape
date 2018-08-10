//
//  ape_mind.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <cassert>
#include <fstream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include "NeurConscious.hpp"
#include "NeuronDef.hpp"


namespace nsAI {
    namespace nsNeuronal{
        
        class CThink::CPrivate : public CObject
        {
        public:
			CPrivate();
            ~CPrivate() final;
			void tense(std::unique_ptr<CEmotion>);
            std::ofstream& log()
            {
                m_log << "[think]";
                return m_log;
            }
		private:

			std::set<size_t> m_tagUndupped;
			std::vector<size_t> m_tagSeq;
			std::shared_ptr<CTagIndex> m_tagIndex;
			std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
			std::shared_ptr<CNeuronPool> m_spNeurPool;
            std::ofstream m_log;
        };
        
		CThink::CPrivate::CPrivate() 
			: m_tagIndex(std::make_shared<CTagIndex>())
			, m_spNeurPool(std::make_shared<CNeuronPool>())
		{
            m_log.open("/Users/alanking/Documents/my_AI/my_github/my_ape/ape_v1/ape_lib/think.log");
		}
        CThink::CPrivate::~CPrivate()
        {
            auto acc = m_spNeurPool->getAccessor();
            auto neurNum =m_spNeurPool->getNeuronalNum();
            log() << "all neuron number : " << neurNum << std::endl;
            auto upCur = acc->getFirst();
            for (size_t i = 0 ; i < neurNum; i++) {
                auto spNeur = acc->getNext(upCur.get());
                log() << "tag[" << spNeur->m_tag << "]" << std::endl;
            }
        }

		void CThink::CPrivate::tense(std::unique_ptr<CEmotion> e)
		{
			log() << "tense " << CEmotion::echo(e->m_tag) << std::endl;

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
						[spDendrite](std::shared_ptr<CNeuron> spN) {
                            auto spAxon = spN->buildAxon();
                            spDendrite->attach(spAxon);
                            spAxon->attach(spDendrite);
                        });
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
	}
}

void nsAI::nsNeuronal::CThink::operator()()
{
    while (m_pCortex->isAlive())
    {
        auto e = m_pConscious->getEmotion();
        if (e)
        {
			mp->tense(std::move(e));
        }
        else
        {
            mp->log() << "idle" << std::endl;
        }
    }
}

