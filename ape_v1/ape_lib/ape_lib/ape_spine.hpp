//
//  ape_spine.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_spine_hpp
#define ape_spine_hpp
#ifdef _WIN32
#include <atomic>
#endif // _WIN32


#include "ape.hpp"
#include "IBusServer.hpp"

class nsAI::CApe::CSpine: public nsAI::nsNeuronal::IBusServer
{
public:
    CSpine() = default;
    ~CSpine() final = default;
    
    void Send(std::unique_ptr<nsAI::nsNeuronal::nsBus::CMessage> m) final;
    void Connect(CConnectiveTarget_E type, nsAI::nsNeuronal::nsBus::CMessageTarget_t* target) final;

private:
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pInput;
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pOutput;
    std::atomic< nsAI::nsNeuronal::nsBus::CMessageTarget_t* > m_pCortex;
};

#endif /* ape_spine_hpp */
