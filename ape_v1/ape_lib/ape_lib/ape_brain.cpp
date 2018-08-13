//
//  ape_brain.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <cassert>
#include <memory>

#include "EmotionTarget.hpp"
#include "IBusServer.hpp"
#include "BusClient.hpp"
#include "NeurInstinct.hpp"
#include "NeurUnconsci.hpp"
#include "NeurConscious.hpp"
#include "NeurCortex.hpp"

#include "ape.hpp"
#include "ape_brain.hpp"
#include "ape_spine.hpp"


nsAI::CApe::CBrain::CBrain(std::shared_ptr<CSpine> spSpine)
//: m_cortex(std::make_unique<nsAI::nsNeuronal::CCortex>())
: m_cortex(new nsNeuronal::CCortex)
{
    assert(spSpine);
    m_cortex->connect(spSpine.get(), nsNeuronal::IBusServer::CConnectiveTarget_E::cortex);
}

void nsAI::CApe::CBrain::Kill()
{
    return m_cortex->Kill();
}
