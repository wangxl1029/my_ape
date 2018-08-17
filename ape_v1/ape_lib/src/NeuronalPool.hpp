//
//  NearonalLayer.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/14.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef NeuronalPool_hpp
#define NeuronalPool_hpp
namespace nsAI{
    namespace nsNeuronal{
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
        
        class CAssociatedNeuronBuilder : public CNoCopyable
        {
        public:
            ~CAssociatedNeuronBuilder() final = default;
            void checkOrBuild(std::shared_ptr<CTagIndex> spIdx,
                              std::function<std::shared_ptr<CNeuron>()> fnbuildNeuron,
                              std::function<void(size_t)> nfSendEmotion);
        private:
            std::map<std::shared_ptr<CTagIndex>, std::shared_ptr<CNeuron> , CSptrLess<CTagIndex> > m_data;
        };
        
        class CNeuronPool : public CNoCopyable
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
    }
}

#endif /* NearonalLayer_hpp */
