// mindloop.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <thread>
#include <queue>
#include <cassert>
#include <mutex>
#include <chrono>
#include <atomic>
#include <string>
#include <tuple>
#include <limits>

namespace nsAI {
	class CObject {
	public:
		virtual ~CObject() = default;
	};

	class CNoCopyable : public CObject
	{
	public:
		CNoCopyable() = default;
		CNoCopyable(const CNoCopyable&) = delete;
		CNoCopyable& operator=(const CNoCopyable&) = delete;
	};


	///< thread-safe queue template
	template<class _T>
	class CQueue_TS
	{
	public:
		CQueue_TS()
		{
		}

		~CQueue_TS()
		{
		}

		std::unique_ptr<_T> Pop(size_t tmout_ms)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			if (m_cv.wait_for(lk, std::chrono::milliseconds(tmout_ms), [this] {return !m_q.empty(); }))
			{
				auto p = std::move(m_q.front());
				m_q.pop();
				return p;
			}

			return nullptr;
		}

		void Push(std::unique_ptr<_T> p)
		{
			std::lock_guard<std::mutex> _(m_mutex);
			m_q.emplace(std::move(p));
			m_cv.notify_one();
		}

	private:
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::queue< std::unique_ptr<_T> > m_q;
	};

	template<class _MsgT>
	class CActiveTarget : public CNoCopyable
	{
	public:
		CActiveTarget()
		{
		}

		~CActiveTarget() override
		{
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}

		std::unique_ptr<_MsgT> PopMessage(size_t tmout_ms = 100)
		{
			return m_mq.Pop(tmout_ms);
		}

		void PushMessage(std::unique_ptr< _MsgT > m)
		{
			return m_mq.Push(std::move(m));
		}

		virtual void Kill() = 0;
	protected:
		std::thread m_thread;
	private:
		CQueue_TS<_MsgT> m_mq;
	};

	namespace nsNeuronal
	{
		namespace nsBus 
		{
			enum class CMessageId_E
			{
				CORTEX_TEST,
				CORTEX_TEXT_INPUT,
				CORTEX_IDLE_INPUT,
				CORTEX_ERROR,
				INPUT_TEST,
				INPUT_ERROR,
				NONE
			};

			class CMessage : public CObject
			{
			public:
				CMessage() : CMessage(CMessageId_E::NONE) {}
				CMessage(CMessageId_E id) : m_ID(id)
				{
				}

				~CMessage() override
				{
				}

				const CMessageId_E m_ID;
				std::unique_ptr<CObject> m_upData;
			};

			class CMsgIdleInput : public CMessage
			{
			public:
				CMsgIdleInput() : CMessage(CMessageId_E::CORTEX_IDLE_INPUT)
				{
				}

				~CMsgIdleInput()
				{
				}

				static std::unique_ptr<CMsgIdleInput> CreateUniquePtr()
				{
					return std::make_unique<CMsgIdleInput>();
				}
			};

			class CMsgText : public CMessage
			{
			public:
				class CData : public CObject
				{
				public:
					CData(const char* s); // s must NOT be nullptr
					~CData() final;

				public:
					std::string m_text;
				};

				CMsgText() : CMessage(CMessageId_E::CORTEX_TEXT_INPUT)
				{
				}

				CMsgText(const char* s) : CMessage(CMessageId_E::CORTEX_TEXT_INPUT)
				{
					if (s)
					{
						m_upData = std::make_unique<CData>(s);
					}
				}

				~CMsgText() final
				{
				}

				static std::unique_ptr<CMsgText> CreateUniquePtr(const char* s)
				{
					return s ? std::make_unique<nsNeuronal::nsBus::CMsgText>(s) : std::make_unique<nsNeuronal::nsBus::CMsgText>();
				}

				static std::unique_ptr<CData> getDataUniquePtr(std::unique_ptr<CObject> upData)
				{
					if (upData)
					{
						auto p = dynamic_cast<CData*>( upData.get() );
						if (p)
						{
							upData.release();
							return std::unique_ptr<CData>(p);
						}
						else
						{
							std::cerr << "base/derived not matched" << std::endl;
						}
					}
					return nullptr;
				}
			};

			CMsgText::CData::CData(const char* s)
			{
				assert(s);
				m_text = s;
			}

			CMsgText::CData::~CData()
			{
			}

		}

		class IBusServer
		{
		public:
			enum class CConnectiveTarget_E
			{
				cortex, input, output
			};
			virtual void Send(std::unique_ptr<nsBus::CMessage> m) = 0;
			virtual void Connect(CConnectiveTarget_E type, CActiveTarget<nsBus::CMessage>* target) = 0;
			virtual ~IBusServer() = default;
		};


		class CBusProxy : public CNoCopyable
		{
		public:
			CBusProxy() : m_pBusServer(nullptr)
			{
			}

			~CBusProxy() final
			{
			}

			CBusProxy& operator=(IBusServer* right)
			{
				m_pBusServer.store(right);
				return *this;
			}

			void Send(std::unique_ptr<nsBus::CMessage> m)
			{
				static thread_local auto pBus = m_pBusServer.load();

				if (nullptr == pBus)
				{
					pBus = m_pBusServer.load();
				}

				if (pBus)
				{
					pBus->Send(std::move(m));
				}
			}
		private:
			std::atomic<IBusServer*> m_pBusServer;
		};

		class CBusServer : protected CActiveTarget<nsBus::CMessage>, public IBusServer
		{
		public:

			CBusServer()
			{
			};

			~CBusServer() final
			{
			}

			void Send(std::unique_ptr<nsBus::CMessage> m) final
			{
				CActiveTarget* target = nullptr;
				if ( m->m_ID >= nsBus::CMessageId_E::CORTEX_TEST && m->m_ID < nsBus::CMessageId_E::CORTEX_ERROR )
				{
					target = m_pCortex.load();
				}
				else if (m->m_ID >= nsBus::CMessageId_E::INPUT_TEST && m->m_ID < nsBus::CMessageId_E::INPUT_ERROR)
				{
					target = m_pInput.load();
				}

				if (target)
				{
					target->PushMessage(std::move(m));
				}
			}

			void Connect(CConnectiveTarget_E type, CActiveTarget* target) final
			{
				switch (type)
				{
				case IBusServer::CConnectiveTarget_E::cortex:
					m_pCortex.store( target );
					break;
				case IBusServer::CConnectiveTarget_E::input:
					m_pInput.store( target );
					break;
				case IBusServer::CConnectiveTarget_E::output:
					m_pOutput.store(target);
					break;
				default:
					std::cerr << "Unknown target" << std::endl;
					break;
				}
			}

			void Kill() final
			{
			}

		private:
			std::atomic< CActiveTarget*> m_pInput;
			std::atomic< CActiveTarget*> m_pOutput;
			std::atomic< CActiveTarget*> m_pCortex;
		};   

		class CBusClient : protected CActiveTarget<nsBus::CMessage>
		{
		public:
			CBusClient()
			{
				m_isAlive = true;
			}

			~CBusClient() override
			{
			}

			void connect(CBusServer* bus, CBusServer::CConnectiveTarget_E target)
			{
				assert(bus);
				bus->Connect(target, this);
				m_bus = bus;
				
			}

			bool isAlive() const
			{
				return m_isAlive.load();
			}

			std::unique_ptr<nsNeuronal::nsBus::CMessage> getmsg()
			{
				return PopMessage();
			}

			CBusProxy& bus()
			{
				return m_bus;
			}

			void Kill() override
			{
				std::atomic_store(&m_isAlive, false);
			}
		protected:

			std::atomic_bool m_isAlive;
			CBusProxy m_bus;
		};

		enum class CEmotion_E
		{
			input_absence,
			input_test,
			input_txt,
			instinct_crying,
			instinct_sleep,
			instinct_idle,
			max
		};

		enum class CSensible_tag : size_t
		{
			input_idle,
			input_test		};

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


		class CEmotion : public CObject
		{
		public:
			static const size_t EMOTION_E_MAX = static_cast<size_t>(CEmotion_E::max);
			CEmotion(CEmotion_E t) : m_tag(static_cast<size_t>(t))
			{

			}

			~CEmotion() override
			{
			}
			
			static std::string echo(size_t tagval)
			{
				std::string s = "tag unkown";
				if (tagval < EMOTION_E_MAX)
				{
					switch (static_cast<CEmotion_E>(tagval))
					{
					case CEmotion_E::input_absence:
						s = "input absense";
						break;
					case CEmotion_E::input_txt:
						s = "input text";
						break;
					case CEmotion_E::instinct_sleep:
						s = "instinct sleep";
						break;
					case CEmotion_E::instinct_crying:
						s =  "instinct crying";
						break;
					case CEmotion_E::instinct_idle:
						s = "instinct idle";
						break;
					default:
						break;
					}
				}

				return s;
			}

			bool isNotConditional() const
			{
				return m_tag < EMOTION_E_MAX;
			}
			const size_t m_tag;
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

				if ( ! Reflect(tag))
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

				if ( ! isAvailablePreNeuron())
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

		class CEmotionTarget : public  CActiveTarget<CEmotion>
		{
		public:
			CEmotionTarget()
			{
			}

			~CEmotionTarget() override
			{
			}

			void Send(std::unique_ptr<CEmotion> e)
			{
				PushMessage(std::move(e));
			}

			std::unique_ptr<CEmotion> getEmotion()
			{
				return PopMessage();
			}

			void Kill() final
			{
				// can not interfer my sycho
			}
		private:

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

		class CSensor : public CUnconscious
		{
		public:
			CSensor()
			{
			}

			~CSensor() final
			{
			}

			void Activate(CBusClient* parent, CEmotionTarget* mind, CMemory* mem)
			{
				m_thread = std::thread(Reflex, parent, this, mind, mem);
			}

			void SenseNone()
			{
				std::cout << "sensor : sense none" << std::endl;
			}
		private:

			static void Reflex(CBusClient* parent, CSensor* owner, CEmotionTarget* mind, CMemory* mem)
			{
				while (parent->isAlive())
				{
					auto e = owner->getEmotion();
					if (e)
					{
						if (e->m_tag < CEmotion::EMOTION_E_MAX)
						{
							auto tag = static_cast<CEmotion_E>(e->m_tag);
							switch (tag)
							{
							case nsAI::nsNeuronal::CEmotion_E::input_absence:
								break;
							case nsAI::nsNeuronal::CEmotion_E::input_test:
								break;
							case nsAI::nsNeuronal::CEmotion_E::input_txt:
								break;
							case nsAI::nsNeuronal::CEmotion_E::instinct_crying:
								break;
							case nsAI::nsNeuronal::CEmotion_E::instinct_sleep:
								break;
							case nsAI::nsNeuronal::CEmotion_E::instinct_idle:
								break;
							case nsAI::nsNeuronal::CEmotion_E::max:
								break;
							default:
								break;
							}
						}

						std::cout << "sensor: " << CEmotion::echo(e->m_tag) << std::endl;
#if 0
						auto toTransefer = mem->flect(std::move(e));
						if (toTransefer)
						{
							mind->Send(std::move(toTransefer));
						}

#endif // 0
					}
					else
					{
						owner->SenseNone();
					}
				}
			}

		};

		class CMind : public CEmotionTarget
		{
		public:
			CMind()
			{
			}

			~CMind() final
			{
			}

			void ComeToSense(CBusClient* parent, CMemory *mem)
			{
				m_thread = std::thread(Think, parent, this, mem);
			}

		private:
			static void Think(CBusClient* parent, CEmotionTarget* owner, CMemory *mem)
			{
				while (parent->isAlive())
				{
				}
			}
		};

		class CInstinct : CObject
		{
		public:
			static const size_t HANGERY_MAX = 100;
			static const size_t CRYING_MAX = 100;
			CInstinct():m_pOwner(nullptr)
				, m_pSensor(nullptr)
				, m_isSleeping(false)
				, m_isTired(false)
				, m_nHungery(0)
				, m_nCrying(0)
			{
			}

			~CInstinct() final
			{
			}

			void Initialize(CBusClient* owner, CEmotionTarget* sensor)
			{
				m_pOwner = owner;
				m_pSensor = sensor;
			}

			void operator()()
			{
				assert(m_pOwner);

				while (m_pOwner->isAlive())
				{
					bool isEnableWaken = true;
					auto msg = m_pOwner->getmsg();
					if (msg)
					{
						switch (msg->m_ID)
						{
						case nsBus::CMessageId_E::CORTEX_TEST:
							if ( ! m_isSleeping)
							{
								m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_test));
							}
							break;

						case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
							if ( ! m_isSleeping)
							{
								m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_absence));
								if (m_nHungery++ <= HANGERY_MAX)
								{
									m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_crying));
								}

								if (m_nCrying++ <= CRYING_MAX)
								{
								}
								else
								{
									m_isSleeping = m_isTired = true;
									m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_sleep));
								}
							}
							else
							{
								isEnableWaken = false;
							}	
							break;

						case nsBus::CMessageId_E::CORTEX_TEXT_INPUT:
							if ( ! m_isSleeping)
							{
								m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::input_txt));
							}
							break;

						default:
							break;
						}


					}
					else
					{
						//std::cout << "instinct idle" << std::endl;
						m_pSensor->Send(std::make_unique<CEmotion>(CEmotion_E::instinct_idle));
					}

					if (m_isSleeping && m_isTired)
					{
						relex();
					}

					if (m_isSleeping && ! m_isTired && isEnableWaken)
					{
						m_isSleeping = false;
					}
				}
			}

		private:
			inline void relex()
			{
				if (m_nHungery > 0)
				{
					if(0 == m_nHungery--)
						m_isTired = false;
				}

				if (m_nCrying > 0)
				{
					m_nCrying = 0;
				}

			}
			size_t			m_nHungery;
			size_t			m_nCrying;
			bool			m_isTired;
			bool			m_isSleeping;
			CBusClient*		m_pOwner;
			CEmotionTarget* m_pSensor;
		};


		class CCortex : public CBusClient
		{
		public:
			CCortex()
			{
				m_instinct.Initialize(this, &m_Sensor);
				m_thread = std::thread(m_instinct);
				m_Sensor.Activate(this, &m_Mind, &m_Memory);
			}

			~CCortex() final
			{
			}

		private:
#if 0
			void SendInputIdleEmotion(CEmotionTarget& sycho)
			{
				sycho.Send(std::make_unique<CEmotion>(CEmotion_E::input_absence));
				sycho.Send(std::make_unique<CEmotion>(CEmotion_E::instinct_crying));
			}

			static void Instinct(CCortex* owner)
			{
				assert(owner);
				std::cout << "come back to scense." << std::endl;

				auto &rSensor = owner->m_Sensor;
				auto &rMind = owner->m_Mind;
				auto &rMem = owner->m_Memory;
				nsSycho::CAssociateNet net;

				rSensor.Activate(owner, &owner->m_Mind, &owner->m_Memory);
				rMind.ComeToSense(owner, &owner->m_Memory);
				rMem.Load(net);

				while (owner->isAlive())
				{
					auto msg = owner->getmsg();
					if (msg)
					{
						auto some_sense = rMem.BuildReflect(std::move(msg));
						rSensor.Send(std::move(some_sense));
						switch (msg->m_ID)
						{
						case nsBus::CMessageId_E::CORTEX_TEST:
							std::cout << "ape cortex test" << std::endl;
							if (owner->pBus())
							{
								owner->pBus()->send(std::make_unique<nsNeuronal::nsBus::CMessage>(nsNeuronal::nsBus::CMessageId_E::INPUT_TEST));
							}
							break;
						case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
							//owner->SendInputIdleEmotion(rSensor);
							//owner->se
							//rSensor.Send()
							break;
						case nsBus::CMessageId_E::CORTEX_TEXT_INPUT:

							if (auto upData = nsNeuronal::nsBus::CMsgText::getDataUniquePtr(std::move(msg->m_upData)))
							{
								std::cout << "ape cortex txt(" << upData->m_text << ") arrived" << std::endl;
							}
							else
							{

								std::cout << "ape cortex txt(nullptr) arrived" << std::endl;
							}
							break;
						default:
							break;
						}
					}
				}

				std::cout << "lose sense." << std::endl;
			}

#endif // 0

			CMind m_Mind;
			CMemory m_Memory;
			CSensor m_Sensor;
			CInstinct m_instinct;
		};
	}

	class CInput : public nsNeuronal::CBusClient
	{
	public:
		CInput()
		{
			m_thread = std::thread(process, this);
		}

		~CInput() final
		{
		}

		void read(const char* txt)
		{
			bus().Send(nsNeuronal::nsBus::CMsgText::CreateUniquePtr(txt));
		}

		void test()
		{
			bus().Send(std::make_unique<nsNeuronal::nsBus::CMessage>(nsNeuronal::nsBus::CMessageId_E::CORTEX_TEST));
		}

		void connect(nsNeuronal::CBusServer* bus)
		{
			CBusClient::connect(bus, nsNeuronal::CBusServer::CConnectiveTarget_E::input);
		}

	private:
		static void process(CBusClient* owner)
		{
			auto &Bus = owner->bus();
			while (owner->isAlive())
			{
				auto msg = owner->getmsg();
				if (msg)
				{
					switch (msg->m_ID)
					{
					case nsNeuronal::nsBus::CMessageId_E::INPUT_TEST:
						std::cout << "ape input test" << std::endl;
						break;
					default:
						break;
					}
				}
				else
				{
					Bus.Send(nsNeuronal::nsBus::CMsgIdleInput::CreateUniquePtr());
				}
			}
		}

	};

	class COutput : public nsNeuronal::CBusClient
	{
	public:
		COutput()
		{
		}

		~COutput() final
		{
		}

		void connect(nsNeuronal::CBusServer* bus)
		{
			CBusClient::connect(bus, nsNeuronal::CBusServer::CConnectiveTarget_E::output);
		}
	private:

		static void process(CBusClient* owner)
		{
			auto &Bus = owner->bus();
			while (owner->isAlive())
			{
				auto msg = owner->getmsg();
				if (msg)
				{
					switch (msg->m_ID)
					{
					case nsNeuronal::nsBus::CMessageId_E::INPUT_TEST:
						std::cout << "ape input test" << std::endl;
						break;
					default:
						break;
					}
				}
				else
				{
					Bus.Send(nsNeuronal::nsBus::CMsgIdleInput::CreateUniquePtr());
				}
			}
		}

	};


	class CBrain : public CObject
	{
	public:
		CBrain()
		{
			std::cout << "enter brain ctor." << std::endl;
		}

		~CBrain()
		{
		}

		void Kill()
		{
			m_cortex.Kill();
		}

		void connect(nsNeuronal::CBusServer* bus)
		{
			m_cortex.connect(bus, nsNeuronal::CBusServer::CConnectiveTarget_E::cortex);
		}

	private:

		nsNeuronal::CCortex m_cortex;
	};

	class CApe : public CObject
	{
	public:
		CApe();
		~CApe() override;
		
		void read(const char*);
		void kill();
	private:
		
		CBrain	m_brain;
		CInput	m_input;
		COutput	m_output;
		nsNeuronal::CBusServer m_NeurBus;
	};

	CApe::CApe()
	{
#if 1
		m_input.connect(&m_NeurBus);
		m_output.connect(&m_NeurBus);
		m_brain.connect(&m_NeurBus);
#else
#endif // 0
	}

	CApe::~CApe()
	{
	}
}

void nsAI::CApe::read(const char* txt)
{
	m_input.read(txt);
}

void nsAI::CApe::kill()
{
	m_brain.Kill();
	m_input.Kill();
}


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

