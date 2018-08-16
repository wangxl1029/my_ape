//
//  neur_priv.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
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
#include "NeurLayer.hpp"

#ifdef _WIN32
using namespace nsAI;
#endif // _WIN32
using namespace nsAI::nsNeuronal;

CLayerLifeCycle::CLayerLifeCycle()
{
	Reset(true);
}

void CLayerLifeCycle::Reset(bool val = false)
{
	m_alive.store(val);
}

bool CLayerLifeCycle::isAlive()
{
	return m_alive.load();
}


class CLayerWork::CPrivate : public CNoCopyable
{
public:
    ~CPrivate() final = default;
};

CLayerWork::CLayerWork(ILifeCycle& lc, CLayer& owner)
:m_owner(owner), m_lc(lc) , m_pNext(nullptr)
{

}

void CLayerWork::operator()(CLayerGenerator* pGen)
{
	while (m_lc.isAlive()) {
        bool isAssociated = false;
        if (isAssociated) {
            m_pNext = pGen->getNewLayer(m_lc);
            //m_pNext->Send( )
        };
        auto e = m_owner.PopMessage();
        if (e)
        {
            //mp->Motivate(std::move(e));
        }
        else
        {
            //mp->log() << "idle" << std::endl;
        }
	}
}

class CLayer::CPrivate : public CNoCopyable
{
public:
	CPrivate() = default;
};

CLayer::CLayer() : CLayer(std::thread())
{}

CLayer::CLayer(std::thread &&t) : mp(std::make_shared<CPrivate>())
{
    m_thread = std::move(t);
}

CLayer& CLayer::operator=(std::thread && t)
{
    m_thread = std::move(t);
    return *this;
}

CLayer* CLayerGenerator::getNewLayer()
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>();
    m_vecLayers.push_back(spLayer);
    return spLayer.get();
}

CLayer* CLayerGenerator::getNewLayer(ILifeCycle& lc)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>();
    m_vecLayers.push_back(spLayer);
    
    auto spWork = std::make_shared<CLayerWork>(lc, *spLayer);
    m_vecWork.push_back(spWork);
    *spLayer = std::thread(*spWork, this);
    
    return spLayer.get();
}

CLayer& CLayerGenerator::getNewLayer(std::thread&& t)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(std::move(t));
    m_vecLayers.push_back(spLayer);
    return *spLayer;
}



CLayerWork* CLayerGenerator::getNewWork(nsAI::ILifeCycle &lc, CLayer &owner)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spWork = std::make_shared<CLayerWork>(lc, owner);
    m_vecWork.push_back(spWork);
    return spWork.get();
}
