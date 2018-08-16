//
//  ape_output.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_output_hpp
#define ape_output_hpp

class nsAI::CApe::COutput: public nsAI::nsNeuronal::CBusClient
{
public:
    COutput() = delete;
    COutput(std::shared_ptr<CSpine>);
    ~COutput() final = default;
private:
    static void process(CBusClient* owner);
};

#endif /* ape_output_hpp */
