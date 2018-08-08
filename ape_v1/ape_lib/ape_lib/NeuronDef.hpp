#pragma once
#include <set>
#include "NeurDendrite.hpp"
#include "NeurAxon.hpp"
namespace nsAI {
	namespace nsNeuronal {
		
		class CNeuron : public CObject
		{
		public:
			CNeuron();
			CNeuron(size_t tag);
			~CNeuron() override = default;
			size_t strengthen() { return m_strongVal++; }
			std::shared_ptr< CDendrite > buildDendrite();
			std::shared_ptr< CAxon > buildAxon();

			struct SPtrLess {
				bool operator()(std::shared_ptr<CNeuron>, std::shared_ptr<CNeuron>) const;
			};
			static bool less(const CNeuron&, const CNeuron&);
			static bool less(std::shared_ptr<CNeuron>, std::shared_ptr<CNeuron>);
			const size_t m_tag;
		private:
			size_t m_strongVal;
			std::vector< std::shared_ptr< CAxon > > m_vecAxon;
			std::vector< std::shared_ptr< CDendrite > > m_vecDendrite;
		};

		class CTagIndex : public CObject
		{
		public:
			typedef std::vector< size_t > TagVec_t;
			typedef std::shared_ptr<TagVec_t> TagVec_sptr;
			~CTagIndex() final = default;
			bool Insert(TagVec_sptr);
		private:
			struct TagVecSptrLess {
				bool operator()(TagVec_sptr lhs, TagVec_sptr rhs) const;
			};
			std::set< TagVec_sptr, TagVecSptrLess > m_data;
		};

		class CNeuronPool : public CObject
		{
		public:
			~CNeuronPool() final = default;
			std::shared_ptr<CNeuron> buildNeuron(size_t tag);
		private:
			std::set< std::shared_ptr<CNeuron>, CNeuron::SPtrLess > m_data;
		};

		class CAssociateBuilder : public CObject
		{
		public:
			CAssociateBuilder() = delete;
			CAssociateBuilder(std::shared_ptr<CTagIndex>spIdx, std::shared_ptr<CNeuronPool> spPool);
			~CAssociateBuilder() final;
			void add(std::shared_ptr<CNeuron>);
		private:
			std::shared_ptr<CTagIndex> m_spTagIdx;
			std::shared_ptr<CNeuronPool> m_spNeurPool;
			std::vector< std::shared_ptr< CNeuron > > m_vecNeuron;
		};

	}
}