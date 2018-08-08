#include <algorithm>
#include "NeuronDef.hpp"
#include "NeurEmotion.hpp"

namespace nsAI {
	namespace nsNeuronal {

		inline CNeuron::CNeuron() :CNeuron(SIZE_T_MAX)
		{
		}

		CNeuron::CNeuron(size_t tag) : m_tag(tag), m_strongVal(0)
		{
		}

		std::shared_ptr<CDendrite> CNeuron::buildDendrite()
		{
			m_vecDendrite.push_back(std::make_shared<CDendrite>());
			return m_vecDendrite.back();
		}

		std::shared_ptr<CAxon> CNeuron::buildAxon()
		{
			m_vecAxon.push_back(std::make_shared<CAxon>());
			return m_vecAxon.back();
		}

		bool CNeuron::less(const CNeuron &lhs, const CNeuron &rhs)
		{
			return lhs.m_tag < rhs.m_tag;
		}

		bool CNeuron::less(const std::shared_ptr<CNeuron> lhs, const std::shared_ptr<CNeuron> rhs)
		{
			return less(*lhs, *rhs);
		}

		bool CNeuron::SPtrLess::operator()(std::shared_ptr<CNeuron> lhs, std::shared_ptr<CNeuron> rhs) const
		{
			return less(lhs, rhs);
		}

		inline CAssociateBuilder::CAssociateBuilder(std::shared_ptr<CTagIndex> spIdx, std::shared_ptr<CNeuronPool> spPool)
			: m_spTagIdx(spIdx)
			, m_spNeurPool(spPool)
		{}

		CAssociateBuilder::~CAssociateBuilder()
		{
			auto spTags = std::make_shared< CTagIndex::TagVec_t >();

			std::for_each(m_vecNeuron.begin(), m_vecNeuron.end(), 
				[spTags](std::shared_ptr<CNeuron> spN) {spTags->push_back(spN->m_tag); });

			if (m_spTagIdx->Insert(spTags))
			{
				auto newNeur = m_spNeurPool->buildNeuron(CEmotion::getUniqueTag());
				auto spDendrite = newNeur->buildDendrite();

				std::for_each(m_vecNeuron.begin(), m_vecNeuron.end(), 
					[spDendrite](std::shared_ptr<CNeuron> spN) {spDendrite->attach(spN->buildAxon()); });
			}
		}

		void CAssociateBuilder::add(std::shared_ptr<CNeuron> spNeur)
		{
			m_vecNeuron.push_back(spNeur);
		}

		bool CTagIndex::TagVecSptrLess::operator()(TagVec_sptr lhs, TagVec_sptr rhs) const
		{
			bool isLess = lhs->size() < rhs->size();
			if (lhs->size() == rhs->size())
			{
				size_t i = 0;
				for (; i < lhs->size() && lhs->at(i) == rhs->at(i); i++)
					;// do nothing
				if (i < lhs->size())
				{
					isLess = lhs->at(i) < rhs->at(i);
				}
			}

			return isLess;
		}
		bool CTagIndex::Insert(TagVec_sptr spTagVec)
		{
			return m_data.insert(spTagVec).second;
		}

		std::shared_ptr<CNeuron> CNeuronPool::buildNeuron(size_t tag)
		{
			auto ret_pair = m_data.emplace(std::make_shared<CNeuron>(tag));
			return *ret_pair.first;
		}
	}
}