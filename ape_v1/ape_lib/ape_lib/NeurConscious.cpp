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
			std::shared_ptr<CEmoCached> buildCached(size_t tag);
			void buildAssociated(std::shared_ptr<CNeuron> spNeur);
			bool isCached(size_t tag);
			void tense(std::unique_ptr<CEmotion>);
		private:
			std::shared_ptr<CNeuron> getPreNeuron() const;

			std::shared_ptr<CTagIndex> m_tagIndex;
			std::unordered_set<size_t> m_cachedTags;
			std::set< std::shared_ptr<CNeuron>, CNeuron::SPtrLess > m_neuronalPool;
			std::shared_ptr<CNeuronPool> m_spNearPool;
        };
        
		CThink::CPrivate::CPrivate() 
			: m_tagIndex(std::make_shared<CTagIndex>())
			, m_spNearPool(std::make_shared<CNeuronPool>())
		{
		}

		inline std::shared_ptr<CEmoCached> CThink::CPrivate::buildCached(size_t tag)
		{ 
			m_cachedTags.emplace(tag);
			return std::make_shared<CEmoCached>(buildNeuron(tag));
		}

		inline void CThink::CPrivate::buildAssociated(std::shared_ptr<CNeuron> curNeur) 
		{
			assert(curNeur);
			auto preNeur = getPreNeuron();
			CAssociateBuilder b(m_tagIndex, m_spNearPool);
			b.add(getPreNeuron());
			b.add(curNeur);
		}

		bool CThink::CPrivate::isCached(size_t tag)
		{
			return m_cachedTags.end() != m_cachedTags.find(tag);
		}

		void CThink::CPrivate::tense(std::unique_ptr<CEmotion> e)
		{
			std::cout << "mind: tense " << CEmotion::echo(e->m_tag) << std::endl;
			auto pCached = buildCached(e->m_tag);
			assert(pCached);

			pCached->strengthen();

			buildAssociated(pCached->getNeuron());
		}

		std::shared_ptr<CNeuron> CThink::CPrivate::getPreNeuron() const
		{
			return std::shared_ptr<CNeuron>();
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

