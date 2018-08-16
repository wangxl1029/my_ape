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
	CPrivate() = default;
	void getNewTarget();
	CLayerProxy& getHeader_TS();

	std::vector< std::shared_ptr<CLayerProxy> > m_vecProxy;
	CLayerLifeCycle m_lifeCycle;
	std::mutex m_mtxProxy;
};

CLayerPool::CLayerPool() : mp(std::make_shared<CPrivate>())
{

}

CLayerProxy& CLayerPool::CPrivate::getHeader_TS()
{
	std::lock_guard<std::mutex> lk(m_mtxProxy);
	if (m_vecProxy.empty()) {
		m_vecProxy.push_back(std::make_shared<CLayerProxy>());
	}

	return *m_vecProxy.front();
}

void CLayerPool::Send(std::unique_ptr<CEmotion> e)
{
	return mp->getHeader_TS().Send_TS(std::move(e));
}
