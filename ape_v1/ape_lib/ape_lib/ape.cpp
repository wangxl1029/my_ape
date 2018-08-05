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
#include "ape_output.hpp"

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

void nsAI::CApe::kill()
{
#if 0
    m_brain.Kill();
    m_input.Kill();
    m_output.Kill();
#else
    m_spBrain->Kill();
#endif
}

