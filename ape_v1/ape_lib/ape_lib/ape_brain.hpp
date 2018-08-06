//
//  ape_brain.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef ape_brain_hpp
#define ape_brain_hpp

#include "ape.hpp"
#include "NeurCortex.hpp"
#include "IBusServer.hpp"

class nsAI::CApe::CBrain: public nsAI::CNoCopyable
{
public:
    CBrain() = delete;
    CBrain(std::shared_ptr<CSpine>);
    ~CBrain() final = default;
    void Kill()
    {
        m_cortex.Kill();
    }
    
private:
    nsNeuronal::CCortex m_cortex;
};

#endif /* ape_brain_hpp */
