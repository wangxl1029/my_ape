//
//  ape_mind.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018年 alan king. All rights reserved.
//
#include <iostream>
#include "NeurMind.hpp"

namespace ns_ = nsAI::nsNeuronal;

void ns_::CMind::Think(ns_::CBusClient* parent, ns_::CEmotionTarget* owner)
{
    while (parent->isAlive())
    {
        auto e = owner->getEmotion();
        if (e)
        {
            std::cout << "mind: " << CEmotion::echo(e->m_tag) << std::endl;
            
        }
    }
}

