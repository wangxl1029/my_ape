//
//  neur_priv.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <memory>
#include <set>
#include <vector>

#include "ai_comm.hpp"
#include "NeurLayer.hpp"

namespace nsAI {
    namespace nsNeuronal{
        bool CTagIndex::operator<(const CTagIndex& rhs) const
        {
            auto &lhs = *this;
            bool isLess = lhs.m_tagSeq.size() < rhs.m_tagSeq.size();
            if (lhs.m_tagSeq.size() == rhs.m_tagSeq.size())
            {
                size_t i = 0;
                for (; i < lhs.m_tagSeq.size() && lhs.m_tagSeq.at(i) == rhs.m_tagSeq.at(i); i++)
                    ;// do nothing
                if (i < lhs.m_tagSeq.size())
                {
                    isLess = lhs.m_tagSeq.at(i) < rhs.m_tagSeq.at(i);
                }
            }
            
            return isLess;
        }
        
        size_t CTagIndex::Size() const
        {
            return m_tagSeq.size();
        }
        
        void CTagIndex::Swap(nsAI::nsNeuronal::CTagIndex & other)
        {
            m_tagSeq.swap(other.m_tagSeq);
        }
        
        bool CTagIndexChecker::Insert(std::shared_ptr<CTagIndex> spIdx)
        {
            return m_indices.insert(spIdx).second;
        }
    }
}
