//
//  ape_cortex.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef ape_cortex_hpp
#define ape_cortex_hpp

#include "BusClient.hpp"
#include "NeurMind.hpp"
#include "NeurInstinct.hpp"
#include "NeurUnconsci.hpp"

namespace nsAI {
    namespace nsNeuronal {
        class CCortex : public CBusClient
        {
        public:
            CCortex()
            {
                m_instinct.Initialize(this, &m_Sensor);
                m_thread = std::thread(m_instinct);
                
//                m_reflect.Intialize(this, &m_Sensor, &m_Mind);
//                m_Sensor = std::thread(m_reflect);
                
//                m_Mind.ComeToSense(this, &m_Memory);
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
            
            CMind m_Mind;
//            CMemory m_Memory;
            CSensor m_Sensor;
            CInstinct m_instinct;
            CReflect m_reflect;
        };

    }
}

#endif /* ape_cortex_hpp */
