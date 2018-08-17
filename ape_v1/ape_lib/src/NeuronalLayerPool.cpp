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
#include <map>
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
    class CLayerLifeCycle : public ILifeCycle
    {
    public:
        CLayerLifeCycle();
        bool isAlive() final;
        void Reset(bool=false);
    private:
        std::atomic_bool m_alive;
    };
    
public:
	CPrivate() {
        m_pRoot = m_gen.getNewLayer(m_lifeCycle);
    };
    CLayer* m_pRoot;
    CLayerGenerator m_gen;
	CLayerLifeCycle m_lifeCycle;
};

CLayerPool::CPrivate::CLayerLifeCycle::CLayerLifeCycle()
{
    Reset(true);
}

void CLayerPool::CPrivate::CLayerLifeCycle::Reset(bool val)
{
    m_alive.store(val);
}

bool CLayerPool::CPrivate::CLayerLifeCycle::isAlive()
{
    return m_alive.load();
}

CLayerPool::CLayerPool() : mp(std::make_shared<CPrivate>())
{

}

void CLayerPool::Send(std::unique_ptr<CEmotion> e)
{
	return mp->m_pRoot->Send(std::move(e));
}

void CLayerPool::Kill()
{
    mp->m_lifeCycle.Reset();
}
