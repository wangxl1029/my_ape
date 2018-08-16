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
#include <fstream>
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
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"
#include "NeuronalPool.hpp"
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
    CPrivate(ILifeCycle& lc, CLayer& owner, CLayerGenerator& gen)
    : m_owner(owner), m_lc(lc) , m_gen(gen), m_pNextLayer(nullptr)
    {}
    
    ~CPrivate() final = default;
    void Motivate(std::unique_ptr<CEmotion>);
    std::ofstream& log()
    {
        m_log << "[think]";
        return m_log;
    }
    
    bool isAlive()
    {
        return m_lc.isAlive();
    }
    
    std::unique_ptr<CEmotion> getEmotion()
    {
        return m_owner.PopMessage();
    }
    
    void dump();
private:
    std::set<size_t> m_tagUndupped;
    CTagIndex m_tagIdx;
    std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
    void buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)>);
    // attributes from parents
    ILifeCycle & m_lc;
    CLayer& m_owner;
    CLayerGenerator& m_gen;
    // next layer
    CLayer* m_pNextLayer;
    CLayer& nextlayer(){
        if (! m_pNextLayer) {
            m_pNextLayer = m_gen.getNewLayer(m_lc);
        }
        return *m_pNextLayer;
    }
    std::shared_ptr<CTagIndexChecker> m_spChecker;
    std::shared_ptr<CNeuronPool> m_spNeurPool;
    std::ofstream m_log;
};

CLayerWork::CLayerWork(ILifeCycle& lc, CLayer& owner, CLayerGenerator& gen)
: mp(std::make_shared<CPrivate>(lc, owner, gen))
{

}

void CLayerWork::operator()()
{
	while (mp->isAlive()) {
        auto e = mp->getEmotion();
        if (e)
        {
            mp->Motivate(std::move(e));
        }
        else
        {
            mp->log() << "idle" << std::endl;
        }
	}
}

class CLayer::CPrivate : public CNoCopyable
{
public:
	~CPrivate() final = default;
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
    
    auto spWork = std::make_shared<CLayerWork>(lc, *spLayer, *this);
    m_vecWork.push_back(spWork);
    *spLayer = std::thread(*spWork);
    
    return spLayer.get();
}

CLayer& CLayerGenerator::getNewLayer(std::thread&& t)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(std::move(t));
    m_vecLayers.push_back(spLayer);
    return *spLayer;
}

void CLayerWork::CPrivate::buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)> spUndupNeur)
{
    assert(spUndupNeur->size() > 1);
    
    auto newNeur = m_spNeurPool->buildNeuron(CEmotion::getUniqueTag());
    auto spDendrite = newNeur->buildDendrite(newNeur);
    std::for_each(spUndupNeur->begin(), spUndupNeur->end(),[spDendrite](std::shared_ptr<CNeuron> spN) {
        auto spAxon = spN->buildAxon(spN);
        spDendrite->attach(spAxon);
        spAxon->attach(spDendrite);
    });
    
    nextlayer().Send(std::make_unique<CEmotion>(newNeur->m_tag));
}

void CLayerWork::CPrivate::Motivate(std::unique_ptr<CEmotion> e)
{
    log() << "emotion tag " << CEmotion::echo(e->m_tag) << std::endl;
    
    auto curNeur = m_spNeurPool->buildNeuron(e->m_tag);
    curNeur->strengthen();
    
    bool ok;
    std::tie(std::ignore, ok) = m_tagUndupped.insert(e->m_tag);
    if (!ok)
    {   // something dupped and clean the undupped tags
        m_tagUndupped.clear();
        
        auto spIdx = std::make_shared<CTagIndex>();
        spIdx->Swap(m_tagIdx); // clean the tag sequence
        
        auto spUndupNeur = std::make_shared<decltype(m_vecUndupNeuron)>();
        spUndupNeur->swap(m_vecUndupNeuron);
        
        if (spIdx->Size() > 1 && m_spChecker->Insert(spIdx)) {
            // check build association
            buildAssoiatedNeuron(spUndupNeur);
        }
        
        m_tagUndupped.insert(e->m_tag);
    }
    
    m_tagIdx.Add(e->m_tag);
    m_vecUndupNeuron.push_back(curNeur);
}

CLayerWork* CLayerGenerator::getNewWork(nsAI::ILifeCycle &lc, CLayer &owner)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spWork = std::make_shared<CLayerWork>(lc, owner, *this);
    m_vecWork.push_back(spWork);
    return spWork.get();
}
