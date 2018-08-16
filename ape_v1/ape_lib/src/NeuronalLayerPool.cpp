//
//  NeuronalLayerPool.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/15.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <algorithm>
#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <set>
#include <vector>


#include "ai_comm.hpp"
#include "ai_active.hpp"
#include "ai_predicate.hpp"
#include "ai_access_imp.hpp"
#include "NeurEmotion.hpp"
#include "EmotionTarget.hpp"
#include "NeuronDef.hpp"
#include "NeuronalPool.hpp"
#include "NeurLayer.hpp"
#include "NeuronalLayerPool.hpp"

using namespace nsAI::nsNeuronal;

class CLayerPool::CPrivate
{
public:
	CPrivate() {
        m_pRoot = m_gen.getNewLayer(m_lifeCycle);
    };
    CLayerGenerator m_gen;
	CLayerLifeCycle m_lifeCycle;
    CLayer* m_pRoot;
	std::mutex m_mtxProxy;
};

CLayerPool::CLayerPool() : mp(std::make_shared<CPrivate>())
{

}

void CLayerPool::Send(std::unique_ptr<CEmotion> e)
{
	return mp->m_pRoot->Send(std::move(e));
}
