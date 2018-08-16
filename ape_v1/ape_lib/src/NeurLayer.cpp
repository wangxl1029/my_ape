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
#include <sstream>
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

#ifdef _WIN32
char log_root[] = R"(C:\Users\win 10\)";
#elif defined(__APPLE__)
char log_root[] = "/Users/alanking/Documents/my_AI/my_github/my_ape/ape_v1/ape_lib/";
#endif // _WIN32

class CLayerWork::CPrivate : public CNoCopyable
{
public:
    CPrivate(ILifeCycle& lc, CLayer& owner, CLayerGenerator& gen)
    : m_owner(owner), m_lc(lc) , m_gen(gen), m_pNextLayer(nullptr)
    , m_spChecker(std::make_shared<CTagIndexChecker>())
    , m_spNeurPool(std::make_shared<CNeuronPool>())
    {
        std::ostringstream os;
        os << log_root << "layer_" << owner.m_tag;
        m_log.open(os.str());
    }
    
    ~CPrivate() final = default;
    void Motivate(std::unique_ptr<CEmotion>);
    std::ofstream& log()
    {
        m_log << "[layer_"<< m_owner.m_tag <<"]";
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
    
    // associated neuron biulding helper
    std::set<size_t> m_tagUndupped;
    CTagIndex m_tagIdx;
    std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
    void buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)>);
    std::shared_ptr<CTagIndexChecker> m_spChecker;
    std::shared_ptr<CNeuronPool> m_spNeurPool;
    
    // log
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
    
    mp->dump();
}

class CLayer::CPrivate : public CNoCopyable
{
public:
	~CPrivate() final = default;
};

CLayer::CLayer(size_t tag) : CLayer(std::thread(), tag)
{}

CLayer::CLayer(std::thread &&thd, size_t tag) : m_tag(tag), mp(std::make_shared<CPrivate>())
{
    m_thread = std::move(thd);
}

CLayer& CLayer::operator=(std::thread && t)
{
    m_thread = std::move(t);
    return *this;
}

CLayer* CLayerGenerator::getNewLayer()
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(m_vecLayers.size());
    m_vecLayers.push_back(spLayer);
    return spLayer.get();
}

CLayer* CLayerGenerator::getNewLayer(ILifeCycle& lc)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(m_vecLayers.size());
    m_vecLayers.push_back(spLayer);
    
    auto spWork = std::make_shared<CLayerWork>(lc, *spLayer, *this);
    m_vecWork.push_back(spWork);
    *spLayer = std::thread(*spWork);
    
    return spLayer.get();
}

CLayer& CLayerGenerator::getNewLayer(std::thread&& t)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(std::move(t), m_vecLayers.size());
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
            //
//            nextlayer().Send(std::make_unique<CEmotion>(newNeur->m_tag));

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

void CLayerWork::CPrivate::dump() {
    log() << ">>> Conscious dumping..." << std::endl;
    log() << "------------8<------------8<------------" << std::endl;
    
    auto upPoolAcc = m_spNeurPool->getAccessor();
    auto neurNum = upPoolAcc->getSize();
    log() << "all neuron number : " << neurNum << std::endl;
    auto upCur = upPoolAcc->getFirst();
    for (size_t i = 0; i < neurNum; i++) {
        auto spNeur = upPoolAcc->getNext(upCur.get());
        auto upAxonAcc = spNeur->getAxonAccessor();
        log() << "tag[" << spNeur->m_tag << "] : " << CEmotion::echo(spNeur->m_tag) << std::endl;
        log() << "\t" << upAxonAcc->getSize() << " axon(s), " << std::endl;
        // log the axon info
        auto upAxonCur = upAxonAcc->getFirst();
        for (size_t i = 0; i < upAxonAcc->getSize(); i++) {
            auto spAxon = upAxonAcc->getNext(upAxonCur.get());
            auto spDadDendri = spAxon->getDendrite();
            auto destTag = spDadDendri->getOwner()->m_tag;
            log() << "\taxon[" << i << "] --> neuron[" << destTag << "] : " << CEmotion::echo(destTag) << std::endl;
        }
        
        auto upDenAcc = spNeur->getDendriAccessor();
        log() << "\t" << upDenAcc->getSize() << " dendrite(s)" << std::endl;
        auto upDendriCur = upDenAcc->getFirst();
        size_t denCnt = 0;
        while (!upDenAcc->isEnded(upDendriCur.get())) {
            auto spDendri = upDenAcc->getNext(upDendriCur.get());
            auto upDenAxnAcc = spDendri->getAxonAccessor();
            log() << "\t\tden[" << denCnt++ << "] <-- " << upDenAxnAcc->getSize() << " axon(s) from neuron(s) by" << std::endl;
            
            auto upDenAxnCur = upDenAxnAcc->getFirst();
            while (!upDenAxnAcc->isEnded(upDenAxnCur.get())) {
                auto spDenAxn = upDenAxnAcc->getNext(upDenAxnCur.get());
                auto spDenAxnOwnerNeur = spDenAxn->getOwner();
                log() << "\t\t\ttag[" << spDenAxnOwnerNeur->m_tag << "] : " << CEmotion::echo(spDenAxnOwnerNeur->m_tag) << std::endl;
            }
        }
    }
}
