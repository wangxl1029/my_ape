//
//  NearonalLayer.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/14.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <algorithm>
#include <functional>
#include <set>
#include <vector>
#include <memory>

#include "ai_comm.hpp"
#include "ai_predicate.hpp"
#include "ai_access_imp.hpp"
#include "NeuronDef.hpp"
#include "NeuronalPool.hpp"

namespace nsAI {
	namespace nsNeuronal {
		bool CTagIndex::operator<(const CTagIndex& rhs) const
		{
			return nsContPred::isLess(this->m_tagSeq, rhs.m_tagSeq);
		}

		size_t CTagIndex::Size() const
		{
			return m_tagSeq.size();
		}

		void CTagIndex::Add(size_t tagval)
		{
			m_tagSeq.push_back(tagval);
		}

		void CTagIndex::Swap(nsAI::nsNeuronal::CTagIndex & other)
		{
			m_tagSeq.swap(other.m_tagSeq);
		}

		size_t CTagIndex::getSingleOne() const
		{
			return m_tagSeq.front();
		}

		bool CTagIndexChecker::Insert(std::shared_ptr<CTagIndex> spIdx)
		{
			return m_indices.insert(spIdx).second;
		}
        
		std::shared_ptr<CNeuron> CNeuronPool::buildNeuron(size_t tag)
		{
			auto ret_pair = m_data.emplace(std::make_shared<CNeuron>(tag));
			return *ret_pair.first;
		}

		std::unique_ptr< CNeuronPool::DataAccessor_t > CNeuronPool::getAccessor()
		{
			return std::make_unique< CAccessor< decltype(m_data) > >(m_data.begin(), m_data.end(), m_data.size());
		}

	}
}
