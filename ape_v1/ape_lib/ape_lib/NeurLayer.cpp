//
//  neur_priv.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
//  Copyright © 2018 alan king. All rights reserved.
//
#include <algorithm>
#include <cassert>
#include <memory>
#include <set>
#include <vector>

#include "ai_comm.hpp"
#include "CAccessor.hpp"
#include "NeuronDef.hpp"
#include "NeurLayer.hpp"

namespace nsAI {
    namespace nsContPred{
        template<class _T>
        inline bool isSizeLess(const _T &lhs, const _T &rhs) {return lhs.size() < rhs.size();}

        template<class _T>
        inline bool isSizeEqual(const _T &lhs, const _T &rhs) {return lhs.size() == rhs.size();}
        
        template<class _T>
        inline bool isLessAsSizequal(const _T &lhs, const _T &rhs){
            auto res_pair = std::mismatch(lhs.begin(), lhs.end(), rhs.begin());
            return  lhs.end() == res_pair.first ? false : (*res_pair.first) < (*res_pair.second);
        }
        
        template<class _T>
        inline bool isLess(const _T &lhs, const _T &rhs){
            return isSizeEqual(lhs, rhs) ? isLessAsSizequal(lhs, rhs) : isSizeLess(lhs, rhs);
        }
    }
    
    namespace nsNeuronal{
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
