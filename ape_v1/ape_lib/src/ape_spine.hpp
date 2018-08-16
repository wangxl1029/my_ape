//
//  ape_spine.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_spine_hpp
#define ape_spine_hpp

class nsAI::CApe::CSpine: public nsAI::nsNeuronal::IBusServer
{
    struct CPrivate;
public:
    CSpine();
    ~CSpine() final = default;
    
    void Send(std::unique_ptr<nsAI::nsNeuronal::nsBus::CMessage> m) final;
    void Connect(CConnectiveTarget_E type, nsAI::nsNeuronal::nsBus::CMessageTarget_t* target) final;
private:
    std::shared_ptr<CPrivate> mp;
};

#endif /* ape_spine_hpp */
