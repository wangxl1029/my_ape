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


namespace nsAI {
	class CObject {
	public:
		virtual ~CObject()
		{

		}
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
	class CActiveTarget : public CObject
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

	namespace nsNeuron
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
					return s ? std::make_unique<nsNeuron::nsBus::CMsgText>(s) : std::make_unique<nsNeuron::nsBus::CMsgText>();
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

		class CBusServer : protected CActiveTarget<nsBus::CMessage>
		{
		public:
			enum class CConnectiveTarget_E
			{
				cortex, input
			};

			CBusServer()
			{
			};

			~CBusServer() final
			{
			}

			void send(std::unique_ptr<nsBus::CMessage> m)
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

			void Connect(CConnectiveTarget_E type, CActiveTarget* target)
			{
				switch (type)
				{
				case CBusServer::CConnectiveTarget_E::cortex:
					m_pCortex.store( target );
					break;
				case CBusServer::CConnectiveTarget_E::input:
					m_pInput.store( target );
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
			std::atomic< CActiveTarget*> m_pCortex;
		};   

		class CBusClient : protected CActiveTarget<nsBus::CMessage>
		{
		public:
			CBusClient()
			{
				m_isAlive = true;
				m_pBus = nullptr;
			}

			~CBusClient() override
			{
			}

			void connect(CBusServer* bus, CBusServer::CConnectiveTarget_E target)
			{
				assert(bus);
				bus->Connect(target, this);
				m_pBus.store(bus);
			}

			bool isAlive() const
			{
				return m_isAlive.load();
			}

			CBusServer* bus() const
			{
				return m_pBus.load();
			}

			std::unique_ptr<nsNeuron::nsBus::CMessage> getmsg()
			{
				return PopMessage();
			}

			void Kill() override
			{
				std::atomic_store(&m_isAlive, false);
			}
		protected:

			std::atomic_bool m_isAlive;
			std::atomic<CBusServer *> m_pBus;
		};

		enum class CEmotion_E
		{
			idle
		};

		class CEmotion : public CObject
		{
		public:
			CEmotion(CEmotion_E eid) : m_eid(eid)
			{

			}

			~CEmotion() override
			{
			}

			const CEmotion_E m_eid;
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

		class CMemory : public CObject
		{
		public:
			CMemory()
			{
			}

			~CMemory() final
			{
			}

			void Load(nsSycho::CAssociateNet&)
			{

			}

		private:

		};

		class CConsciSycho : public  CActiveTarget<CEmotion>
		{
		public:
			CConsciSycho()
			{
			}

			~CConsciSycho() override
			{
			}

			void Kill() final
			{
				// can not interfer my sycho
			}
		private:

		};


		class CUnconscious : public  CConsciSycho
		{
		public:
			CUnconscious()
			{
			}

			~CUnconscious() final
			{
			}

			void Activate(CBusClient* parent, CConsciSycho* mind, CMemory* mem)
			{
				m_thread = std::thread(Reflex, parent, this, mind, mem);
			}

		private:
			static void Reflex(CBusClient* parent, CConsciSycho* owner, CConsciSycho* mind, CMemory* mem)
			{
				while (parent->isAlive())
				{

				}
			}
		};


		class CMind : public CConsciSycho
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
			static void Think(CBusClient* parent, CConsciSycho* owner, CMemory *mem)
			{
				while (parent->isAlive())
				{

				}
			}
		};

		class CCortex : public CBusClient
		{
		public:
			CCortex()
			{
				m_thread = std::thread(Instinct, this);
			}

			~CCortex() final
			{
			}

		private:
			static void Instinct(CCortex* owner)
			{
				assert(owner);
				std::cout << "come back to scense." << std::endl;

				auto &rSense = owner->m_Sense;
				auto &rMind  = owner->m_Mind;
				auto &rMem   = owner->m_Memory;
				nsSycho::CAssociateNet net;

				rSense.Activate(owner, &owner->m_Mind, &owner->m_Memory);
				rMind.ComeToSense(owner, &owner->m_Memory);
				rMem.Load(net);

				while (owner->isAlive())
				{
					auto msg = owner->getmsg();
					if (msg)
					{
						switch (msg->m_ID)
						{
						case nsBus::CMessageId_E::CORTEX_TEST:
							std::cout << "ape cortex test" << std::endl;
							if (owner->bus())
							{
								owner->bus()->send(std::make_unique<nsNeuron::nsBus::CMessage>(nsNeuron::nsBus::CMessageId_E::INPUT_TEST));
							}
							break;
						case nsBus::CMessageId_E::CORTEX_IDLE_INPUT:
							std::cout << "ape idle input" << std::endl;
							break;
						case nsBus::CMessageId_E::CORTEX_TEXT_INPUT:
							
							if (auto upData = nsNeuron::nsBus::CMsgText::getDataUniquePtr(std::move(msg->m_upData)))
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

			CMind m_Mind;
			CMemory m_Memory;
			CUnconscious m_Sense;
		};

	}

	class CInput : public nsNeuron::CBusClient
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
			if (bus())
			{
				bus()->send(nsNeuron::nsBus::CMsgText::CreateUniquePtr(txt));
			}
		}

		void test()
		{
			if (bus())
			{
				bus()->send(std::make_unique<nsNeuron::nsBus::CMessage>(nsNeuron::nsBus::CMessageId_E::CORTEX_TEST));
			}
		}

		void connect(nsNeuron::CBusServer* bus)
		{
			CBusClient::connect(bus, nsNeuron::CBusServer::CConnectiveTarget_E::input);
		}

	private:
		static void process(CBusClient* owner)
		{
			auto pBus = owner->bus();
			while (owner->isAlive())
			{
				auto msg = owner->getmsg();
				if (msg)
				{
					switch (msg->m_ID)
					{
					case nsNeuron::nsBus::CMessageId_E::INPUT_TEST:
						std::cout << "ape input test" << std::endl;
						break;
					default:
						break;
					}
				}
				else
				{
					if ( ! pBus)
					{
						pBus = owner->bus();
					}

					if (pBus)
					{
						pBus->send(nsNeuron::nsBus::CMsgIdleInput::CreateUniquePtr());
					}
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

		void connect(nsNeuron::CBusServer* bus)
		{
			m_cortex.connect(bus, nsNeuron::CBusServer::CConnectiveTarget_E::cortex);
		}

	private:

		nsNeuron::CCortex m_cortex;
	};

	class CApe : public CObject
	{
	public:
		CApe();
		~CApe() override;
		
		void read(const char*);
		void kill();
	private:
		
		CBrain m_brain;
		CInput m_input;
		nsNeuron::CBusServer m_NeurBus;
	};

	CApe::CApe()
	{
		m_input.connect(&m_NeurBus);
		m_brain.connect(&m_NeurBus);
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

