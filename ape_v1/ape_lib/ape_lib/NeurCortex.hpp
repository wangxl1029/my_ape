//
//  ape_cortex.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_cortex_hpp
#define ape_cortex_hpp

#include "BusClient.hpp"
#include "NeurUnconsci.hpp"
#include "NeurConscious.hpp"

namespace nsAI {
    namespace nsNeuronal {
        class CInstinct : CObject
        {
        public:
            static const size_t HANGERY_MAX = 100;
            static const size_t CRYING_MAX = 100;
            
            CInstinct();
            ~CInstinct() final = default;
            
            void Initialize(CBusClient* owner, CEmotionTarget* sensor);
            void operator()();
            
        private:
            inline void relex();
            size_t              m_nHungery;
            size_t              m_nCrying;
            bool                m_isTired;
            bool                m_isSleeping;
            CBusClient*         m_pOwner;
            CEmotionTarget*     m_pUnconsci;
        };
        
        class CCortex : public CBusClient
        {
        public:
            CCortex()
            {
                m_cfnInstinct.Initialize(this, &m_thUnconscious);
                m_thread = std::thread(m_cfnInstinct);
                
                m_cfnReflect.Intialize(this, &m_thUnconscious, &m_thConscious);
                m_thUnconscious = std::thread(m_cfnReflect);
                
                m_cfnThink.initialize(this, &m_thUnconscious, &m_thUnconscious);
                m_thConscious = std::thread(m_cfnThink);
            }
            
            ~CCortex() final = default;
            
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
			// thread
			CUnconscious	m_thUnconscious;
			CConscious		m_thConscious;
			// class of function
            CInstinct		m_cfnInstinct;
            CReflect		m_cfnReflect;
            CThink			m_cfnThink;
        };

    }
}

#endif /* ape_cortex_hpp */
