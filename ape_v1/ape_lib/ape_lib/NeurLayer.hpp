//
//  neur_priv.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef neur_layer_hpp
#define neur_layer_hpp

namespace nsAI {
    template<class _T>
    struct CSptrLess{
        bool operator()(std::shared_ptr<_T> lhs, std::shared_ptr<_T> rhs)
        {
            return (*lhs) < (*rhs);
        }
    };
    
    namespace nsNeuronal
    {
        class CTagIndex : public CNoCopyable
        {
        public:
            ~CTagIndex() final = default;
            bool operator<(const CTagIndex&) const;
            void Add(size_t);
            void Swap(CTagIndex&);
            size_t Size() const;
        private:
            std::vector< size_t > m_tagSeq;
        };
        
        class CTagIndexChecker : public CNoCopyable
        {
        public:
            ~CTagIndexChecker() final = default;
            bool Insert(std::shared_ptr<CTagIndex>);
        private:
            std::set<std::shared_ptr<CTagIndex>, CSptrLess<CTagIndex> > m_indices;
        };
        
        class CNeuronPool : public CObject
        {
        public:
            ~CNeuronPool() final = default;
            std::shared_ptr<CNeuron> buildNeuron(size_t tag);
        private:
            std::set< std::shared_ptr<CNeuron>, CSptrLess<CNeuron> > m_data;
        public:
            typedef IAccessor< decltype(m_data) > DataAccessor_t;
            std::unique_ptr< DataAccessor_t > getAccessor();
        };
        
        class CLayer : public CNoCopyable
        {
        public:
            ~CLayer() final = default;
        };
        
    }
}

#endif /* neur_priv_hpp */
