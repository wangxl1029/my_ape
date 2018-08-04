//
//  ape_lib.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#include <iostream>
#include "ai_priv.hpp"
#include "ape_spine.hpp"
#include "ape_brain.hpp"
#include "ape_input.hpp"

class nsAI::CApe::COutput: public nsAI::CNoCopyable
{
public:
    COutput() = delete;
    COutput(std::shared_ptr<CSpine>) {};
    ~COutput() final = default;
};

nsAI::CApe::CApe()
{
#if 1
    m_spSpine = std::make_shared<CSpine>();
    m_spBrain = std::make_shared<CBrain>(m_spSpine);
    m_spInput = std::make_shared<CInput>(m_spSpine);
    m_spOutput = std::make_shared<COutput>(m_spSpine);
#else
    m_input.connect(&m_NeurBus);
    m_output.connect(&m_NeurBus);
    m_brain.connect(&m_NeurBus);
#endif // 0
}
