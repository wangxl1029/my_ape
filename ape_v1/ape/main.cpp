// mindloop.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>

#include "ai_comm.hpp"
#include "ape.hpp"



#if 0
namespace nsAI {
	namespace nsNeuronal
	{

		enum class CSensible_tag : size_t
		{
			input_idle,
			input_test
		};

		class CSensible :CObject
		{
		public:
			CSensible() = delete;
			CSensible(CSensible_tag t) : m_tag(t)
			{

			}

			~CSensible() override
			{
			}

			const CSensible_tag m_tag;
		private:

		};


		class CReflex : public CEmotion
		{
		public:
			CReflex() = delete;
			CReflex(CEmotion_E e) : CEmotion(e)
			{
			}

			~CReflex() override
			{
			}

		private:

		};



		class CSense : public CEmotion
		{
		public:
			CSense() = delete;
			CSense(CEmotion_E e) : CEmotion(e)
			{
			}

			~CSense() override
			{
			}

		private:

		};


		namespace nsSycho
		{
			class CAssociateNet : public CObject
			{
			public:
				CAssociateNet()
				{
				}

				~CAssociateNet() final
				{
				}

			private:

			};

		}

		class CAxon
		{
		public:
			CAxon() : CAxon(SIZE_MAX) {}
			CAxon(size_t tag) : m_tag(tag), m_StableVal(0), m_tensiveCount(0) {}
			const size_t m_tag;
			size_t Tense() { return m_tensiveCount++; }
			size_t Strengthen()
			{
				return m_StableVal++;
			}
		private:
			size_t m_StableVal;
			size_t m_tensiveCount;
		};

		class CNeuron
		{
		public:
			CNeuron() : m_StableVal(0), m_TensiveCount(0) {}
			void BuildAxon(size_t tag)
			{
				if (nullptr == m_spAxonAll)
				{
					m_spAxonAll = std::make_shared<std::vector<CAxon>>();
				}

				if (!Reflect(tag))
				{
					m_spAxonAll->emplace_back(CAxon(tag));
				}
			}

			size_t Tense()
			{
				return m_TensiveCount++;
			}

			size_t Strengthen()
			{
				return m_StableVal++;
			}

			bool Reflect(const size_t tag)
			{
				auto it = std::find_if(m_spAxonAll->begin(), m_spAxonAll->end(), [tag](CAxon r)->bool {return tag == r.m_tag; });
				bool isFound = (m_spAxonAll->end() != it);
				if (isFound)
				{
					it->Strengthen();
				}
				return isFound;
			}
		private:
			size_t m_StableVal;
			size_t m_TensiveCount;
			std::shared_ptr< std::vector<CAxon> >  m_spAxonAll;
		};

		class CMemory : public CObject
		{
		public:
			///< default value
			static const size_t DEF_NEURON_MAX = static_cast<size_t>(CEmotion_E::max);
			CMemory() : m_preNeuronTag(SIZE_MAX)
			{
				m_spNeurons = std::make_shared<std::vector< std::shared_ptr< CNeuron > > >(DEF_NEURON_MAX);
			}

			~CMemory() final
			{
			}

			void Load(nsSycho::CAssociateNet&)
			{

			}

			void BuildReflex_old(std::unique_ptr<CEmotion> e)
			{
				assert(e);
				return e->isNotConditional() ? biuldNonConditionalReflex_old(e->m_tag) : buildConditionalReflex_old();
			}

			std::unique_ptr<CEmotion> BuildReflect(std::unique_ptr<nsBus::CMessage> m)
			{
				assert(m);
				//return m->isNotConditional() ? buildReflex4noCondition(std::move(m)) : buildReflex4condition(std::move(m));
				return nullptr;
			}

			std::unique_ptr<CEmotion> flect(std::unique_ptr<CEmotion> e)
			{
				bool ok = false;
				if (isAvailablePreNeuron())
				{
					ok = m_spNeurons->at(m_preNeuronTag)->Reflect(e->m_tag);
					m_preNeuronTag = e->m_tag;
				}

				return ok ? nullptr : std::move(e);
			}

			void LogicDeduce()
			{

			}

		private:
			void biuldNonConditionalReflex_old(const size_t tag)
			{
				assert(tag < CEmotion::EMOTION_E_MAX);
				assert(m_spNeurons->size() >= CEmotion::EMOTION_E_MAX);

				if (nullptr == m_spNeurons->at(tag))
				{
					m_spNeurons->at(tag) = std::make_shared<CNeuron>();
					std::cout << "reflex," << CEmotion::echo(tag) << ":created" << std::endl;
				}
				else
				{
					auto stableVal = m_spNeurons->at(tag)->Strengthen();
					std::cout << "reflex," << CEmotion::echo(tag) << ": stable value " << stableVal << std::endl;
				}

				if (!isAvailablePreNeuron())
				{
					m_preNeuronTag = tag;
				}
				else
				{
					auto preNeur = m_spNeurons->at(m_preNeuronTag);
					preNeur->BuildAxon(tag);
				}
			}

			void buildConditionalReflex_old()
			{
			}

			std::unique_ptr<CEmotion> buildReflex4condition(std::unique_ptr<nsBus::CMessage> m)
			{
				return nullptr;
			}

			std::unique_ptr<CEmotion> buildReflex4noCondition(std::unique_ptr<nsBus::CMessage> m)
			{
				return nullptr;
			}

			inline bool isAvailablePreNeuron() const
			{
				return SIZE_MAX != m_preNeuronTag;
			}

			std::size_t m_preNeuronTag;
			std::shared_ptr< std::vector< std::shared_ptr<CNeuron> > >  m_spNeurons;
		};


		class CUnconscious : public  CEmotionTarget
		{
		public:
			CUnconscious()
			{
			}

			~CUnconscious() override
			{
			}

		private:
		};

	}

#endif // 0

int main()
{
	nsAI::CApe ape;

	for (size_t i = 0; i < 100; i++)
	{
		ape.read(nullptr);
		ape.read("hello world");
	}


	std::system("pause");
	ape.kill();

	return 0;
}

