//
//  ape_input.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_input_hpp
#define ape_input_hpp

class nsAI::CApe::CInput : public nsAI::nsNeuronal::CBusClient
{
public:
    CInput() = delete;
    CInput(std::shared_ptr<CSpine>);
    ~CInput() final = default;
    void read(const char* txt);
private:
    static void process(CBusClient* owner);
};


#endif /* ape_input_hpp */
