#include <algorithm>
#include "NeuronDef.hpp"
#include "NeurEmotion.hpp"
#include "neur_priv.hpp"

namespace nsAI {
	namespace nsNeuronal {

        template<class _T>
        class CAccessor : public IAccessor<_T>
        {
        public:
            CAccessor(typename _T::iterator b, typename _T::iterator e)
            : m_begin(b), m_end(e) {}
            ~CAccessor() final = default;
            std::unique_ptr<CNoCopyable> getFirst()
            {
                return std::make_unique<CCursor<_T>>(m_begin);
            }
            typename _T::value_type getNext(CNoCopyable* cursor)
            {
                auto pCur = dynamic_cast<CCursor<_T>*>(cursor);
                return *(pCur->mIt++);
            }
            typename _T::iterator m_begin;
            typename _T::iterator m_end;
        };
        

        
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
        
        std::unique_ptr<CNoCopyable> CNeuronPool::getFirst()
        {
            return std::make_unique<CCursor<decltype(m_data)>>(m_data.begin());
        }
        
        std::shared_ptr<CNeuron> CNeuronPool::getNext(CNoCopyable* cursor)
        {
            auto pCur = dynamic_cast<CCursor<decltype(m_data)>*>(cursor);
            return pCur ? *(pCur->mIt++) : nullptr;
        }
        
        std::unique_ptr< CNeuronPool::DataAccessor_t > CNeuronPool::getAccessor()
        {
            return std::make_unique<CAccessor<decltype(m_data)>>(m_data.begin(), m_data.end());
        }
	}
}
