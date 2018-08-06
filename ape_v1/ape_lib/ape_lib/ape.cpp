//
//  ape_lib.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <iostream>
#include "ai_priv.hpp"
#include "ape_spine.hpp"
#include "ape_brain.hpp"
#include "ape_input.hpp"
#include "ape_output.hpp"

nsAI::CApe::CApe()
{
    m_spSpine = std::make_shared<CSpine>();
    m_spBrain = std::make_shared<CBrain>(m_spSpine);
    m_spInput = std::make_shared<CInput>(m_spSpine);
    m_spOutput = std::make_shared<COutput>(m_spSpine);
}

void nsAI::CApe::kill()
{
    m_spBrain->Kill();
    m_spInput->Kill();
    m_spOutput->Kill();
}

void nsAI::CApe::read(const char* txt)
{
    m_spInput->read(txt);
}
