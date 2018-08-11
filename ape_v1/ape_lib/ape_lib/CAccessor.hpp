//
//  NeurAccessor.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/11.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef CACCESSOR_H
#define CACCESSOR_H
#include "IAccessor.hpp"

namespace nsAI {
    template<class _T>
    struct CCursor : public CNoCopyable
    {
        CCursor(typename _T::iterator it) : mIt(it) {}
        ~CCursor() final = default;
        typename _T::iterator mIt;
    };
    
    template<class _T>
    class CAccessor : public IAccessor<_T>
    {
    public:
        CAccessor(typename _T::iterator b, typename _T::iterator e, typename _T::size_type s)
        : m_begin(b), m_end(e), m_size(s) {}
        ~CAccessor() final = default;
        // access method
        typename _T::size_type getSize() const final {return m_size;}
        std::unique_ptr<CNoCopyable> getFirst()final
        {
            return std::make_unique<CCursor<_T>>(m_begin);
        }
        typename _T::value_type getNext(CNoCopyable* cursor) final
        {
            auto pCur = dynamic_cast<CCursor<_T>*>(cursor);
            return *(pCur->mIt++);
        }
        bool isEnded(CNoCopyable* cursor) const final
        {
            auto pCur = dynamic_cast<CCursor<_T>*>(cursor);
            return m_end == pCur->mIt;
        }
        void reset(CNoCopyable* cursor) final
        {
            dynamic_cast<CCursor<_T>*>(cursor)->mIt = m_begin;
        }
        typename _T::iterator m_begin;
        typename _T::iterator m_end;
        typename _T::size_type m_size;
    };
}


#endif /* NeurAccessor_h */
