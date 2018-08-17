//
//  ai_predicate.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/14.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ai_predicate_h
#define ai_predicate_h

namespace nsAI{
    template<class _T>
    struct CSptrLess{
        bool operator()(std::shared_ptr<_T> lhs, std::shared_ptr<_T> rhs)
        {
            return (*lhs) < (*rhs);
        }
        bool operator()(std::shared_ptr<_T> lhs, std::shared_ptr<_T> rhs) const
        {
            return (*lhs) < (*rhs);
        }
    };

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
}

#endif /* ai_predicate_h */
