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
#include <map>
#include <unordered_set>
#include "NeurConscious.hpp"

namespace nsAI {
    namespace nsNeuronal{
		class CAxon;
		class CDendrite : public CObject
		{
		public: // ctor/dtor
			CDendrite() = default;
			~CDendrite() final = default;
			// property
			size_t getAxonNum() const;
			// modifier
			void attach(std::shared_ptr<CAxon> spAxon);
		private:
			std::vector< std::shared_ptr<CAxon> > m_vecAxon;
		};

		class CAxon : public CObject
		{
		public:
			CAxon()
			{
			}

			~CAxon() final = default;

		private:

		};


		class CNeuron : public CObject
		{
		public:
			CNeuron()
			{
			}

			~CNeuron() override = default;
			void LinkTo(std::shared_ptr<CNeuron> dest);
			size_t strengthen() { return m_strongVal++; }
			std::shared_ptr<CDendrite> buildDendrite();
			std::shared_ptr<CAxon> buildAxon();
		private:
			size_t m_strongVal;
			std::vector< std::shared_ptr<CDendrite> > m_vecDendrite;
		};

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
            ~CPrivate() final = default;
			std::shared_ptr<CEmoCached> buildCached(size_t tag);
			void buildAssociated(std::shared_ptr<CNeuron> spNeur);
			std::shared_ptr<CNeuron> buildNeuron(size_t tag);
			bool isCached(size_t tag);
			void tense(std::unique_ptr<CEmotion>);
		private:
			std::shared_ptr<CNeuron> getPreNeuron() const;

			std::vector<size_t> m_vecCahcedIdx;
			std::unordered_set<size_t> m_cachedTags;
			std::map< size_t, std::shared_ptr<CNeuron> > m_neuronalPool;
        };
        
		inline std::shared_ptr<CEmoCached> CThink::CPrivate::buildCached(size_t tag)
		{ 
			m_cachedTags.emplace(tag);
			return std::make_shared<CEmoCached>(buildNeuron(tag));
		}

		inline void CThink::CPrivate::buildAssociated(std::shared_ptr<CNeuron> curNeur) 
		{
			assert(curNeur);
			auto preNeur = getPreNeuron();
			auto newNeur = buildNeuron(CEmotion::getUniqueTag());
			auto spDendrite = newNeur->buildDendrite();

			spDendrite->attach(preNeur->buildAxon());
			spDendrite->attach(curNeur->buildAxon());
		}

		inline std::shared_ptr<CNeuron> CThink::CPrivate::buildNeuron(size_t tag) 
		{
			auto it = m_neuronalPool.find(tag);
			if (m_neuronalPool.end() == it)
			{
				auto ret_pair = m_neuronalPool.emplace(tag, std::make_shared<CNeuron>());
				if (ret_pair.second)
				{
					it = ret_pair.first;
				}
				else
				{
					std::cerr << "abnormal map emplace" << std::endl;
				}
			}
			assert(m_neuronalPool.end() != it);

			return it->second;
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
			if (m_spNeuron)
			{
				m_spNeuron->strengthen();
			}
		}
		std::shared_ptr<CNeuron> CEmoCached::getNeuron()
		{
			return m_spNeuron;
		}
		void CNeuron::LinkTo(std::shared_ptr<CNeuron> dest)
		{
		}
		std::shared_ptr<CDendrite> CNeuron::buildDendrite()
		{
			return std::make_shared<CDendrite>();
		}
		std::shared_ptr<CAxon> CNeuron::buildAxon()
		{
			return std::make_shared<CAxon>();
		}
		size_t CDendrite::getAxonNum() const
		{
			return m_vecAxon.size();
		}
		void CDendrite::attach(std::shared_ptr<CAxon> spAxon)
		{
			
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

