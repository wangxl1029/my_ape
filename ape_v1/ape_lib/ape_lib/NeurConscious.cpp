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
#include <memory>
#include <set>
#include <unordered_set>
#include <vector>

#include "ai_comm.hpp"
#include "BusClient.hpp"
#include "EmotionTarget.hpp"
#include "CAccessor.hpp"
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"
#include "NeurConscious.hpp"

#include "NeurLayer.hpp"


namespace nsAI {
	namespace nsNeuronal {

        class CThink::CPrivate : public CObject
		{
		public:
			CPrivate();
			~CPrivate() final;
            void Motivate(std::unique_ptr<CEmotion>);
			std::ofstream& log()
			{
				m_log << "[think]";
				return m_log;
			}

			void dump();
		private:

			std::set<size_t> m_tagUndupped;
            CTagIndex m_tagIdx;
			std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
            void buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)>);
		private:
            std::shared_ptr<CTagIndexChecker> m_spChecker;
			std::shared_ptr<CNeuronPool> m_spNeurPool;
			std::ofstream m_log;
		};

		CThink::CPrivate::CPrivate()
            : m_spChecker(std::make_shared<CTagIndexChecker>())
			, m_spNeurPool(std::make_shared<CNeuronPool>())
		{
#ifdef _WIN32
			char log_path[] = R"(C:\Users\win 10\think.log)";
#elif defined(__APPLE__)
			char log_path[] = "/Users/alanking/Documents/my_AI/my_github/my_ape/ape_v1/ape_lib/think.log";
#endif // _WIN32
			m_log.open(log_path);
		}
        
		CThink::CPrivate::~CPrivate()
		{
		}

		void CThink::CPrivate::dump() {
			log() << ">>> Conscious dumping..." << std::endl;
			log() << "------------8<------------8<------------" << std::endl;

			auto upPoolAcc = m_spNeurPool->getAccessor();
			auto neurNum = upPoolAcc->getSize();
			log() << "all neuron number : " << neurNum << std::endl;
			auto upCur = upPoolAcc->getFirst();
			for (size_t i = 0; i < neurNum; i++) {
				auto spNeur = upPoolAcc->getNext(upCur.get());
				auto upAxonAcc = spNeur->getAxonAccessor();
				log() << "tag[" << spNeur->m_tag << "] : " << CEmotion::echo(spNeur->m_tag) << std::endl;
				log() << "\t" << upAxonAcc->getSize() << " axon(s), " << std::endl;
				// log the axon info
				auto upAxonCur = upAxonAcc->getFirst();
				for (size_t i = 0; i < upAxonAcc->getSize(); i++) {
					auto spAxon = upAxonAcc->getNext(upAxonCur.get());
					auto spDadDendri = spAxon->getDendrite();
					auto destTag = spDadDendri->getOwner()->m_tag;
					log() << "\taxon[" << i << "] --> neuron[" << destTag << "] : " << CEmotion::echo(destTag) << std::endl;
				}

				auto upDenAcc = spNeur->getDendriAccessor();
				log() << "\t" << upDenAcc->getSize() << " dendrite(s)" << std::endl;
				auto upDendriCur = upDenAcc->getFirst();
				size_t denCnt = 0;
				while (!upDenAcc->isEnded(upDendriCur.get())) {
					auto spDendri = upDenAcc->getNext(upDendriCur.get());
					auto upDenAxnAcc = spDendri->getAxonAccessor();
					log() << "\t\tden[" << denCnt++ << "] <-- " << upDenAxnAcc->getSize() << " axon(s) from neuron(s) by" << std::endl;

					auto upDenAxnCur = upDenAxnAcc->getFirst();
					while (!upDenAxnAcc->isEnded(upDenAxnCur.get())) {
						auto spDenAxn = upDenAxnAcc->getNext(upDenAxnCur.get());
						auto spDenAxnOwnerNeur = spDenAxn->getOwner();
						log() << "\t\t\ttag[" << spDenAxnOwnerNeur->m_tag << "] : " << CEmotion::echo(spDenAxnOwnerNeur->m_tag) << std::endl;
					}
				}
			}
		}

        void CThink::CPrivate::buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)> spUndupNeur)
        {
            assert(spUndupNeur->size() > 1);
            
            auto newNeur = m_spNeurPool->buildNeuron(CEmotion::getUniqueTag());
            auto spDendrite = newNeur->buildDendrite(newNeur);
            std::for_each(spUndupNeur->begin(), spUndupNeur->end(),
                          [spDendrite](std::shared_ptr<CNeuron> spN) {
                              auto spAxon = spN->buildAxon(spN);
                              spDendrite->attach(spAxon);
                              spAxon->attach(spDendrite);
                          });
        }

         void CThink::CPrivate::Motivate(std::unique_ptr<CEmotion> e)
        {
            log() << "emotion tag " << CEmotion::echo(e->m_tag) << std::endl;
            
            auto curNeur = m_spNeurPool->buildNeuron(e->m_tag);
            curNeur->strengthen();

            bool ok;
            std::tie(std::ignore, ok) = m_tagUndupped.insert(e->m_tag);
            if (!ok)
            {   // something dupped and clean the undupped tags
                m_tagUndupped.clear();
                
                auto spIdx = std::make_shared<CTagIndex>();
                spIdx->Swap(m_tagIdx); // clean the tag sequence
                
                auto spUndupNeur = std::make_shared<decltype(m_vecUndupNeuron)>();
                spUndupNeur->swap(m_vecUndupNeuron);
                
                if (spIdx->Size() > 1 && m_spChecker->Insert(spIdx)) {
                    // check build association
                    buildAssoiatedNeuron(spUndupNeur);
                }
                
                m_tagUndupped.insert(e->m_tag);
            }
            
            m_tagIdx.Add(e->m_tag);
            m_vecUndupNeuron.push_back(curNeur);
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
			mp->Motivate(std::move(e));
		}
		else
		{
			mp->log() << "idle" << std::endl;
		}
	}

	mp->dump();
}

