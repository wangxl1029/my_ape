//
//  ape_brain.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <cassert>
#include "ape.hpp"
#include "ape_brain.hpp"
#include "ape_spine.hpp"

nsAI::CApe::CBrain::CBrain(std::shared_ptr<CSpine> spSpine)
{
    assert(spSpine);
    m_cortex.connect(spSpine.get(), nsNeuronal::IBusServer::CConnectiveTarget_E::cortex);
}


