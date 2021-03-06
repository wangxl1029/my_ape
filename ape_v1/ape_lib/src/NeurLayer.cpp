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
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <set>
#include <vector>

#include "ai_comm.hpp"
#include "ai_access_imp.hpp"
#include "ai_active.hpp"
#include "ai_predicate.hpp"
#include "ai_prop.hpp"
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
//char log_root[] = R"(C:\Users\win 10\)";
char log_root[] = R"(.\)";
#elif defined(__APPLE__)
//char log_root[] = "/Users/alanking/Documents/my_AI/my_github/my_ape/ape_v1/ape_lib/";
char log_root[] = "./";
#endif // _WIN32

class CLayerWork::CPrivate : public CNoCopyable
{
public:
    CPrivate(ILifeCycle& lc, CLayer& owner, CLayerGenerator& gen, std::function<void(std::unique_ptr<CEmotion>)> fnTrans)
    : m_owner(owner), m_lc(lc) , m_gen(gen), m_pNextLayer(nullptr), m_fnTransEmotion(fnTrans)
    , m_spChecker(std::make_shared<CTagIndexChecker>())
    , m_spNeurPool(std::make_shared<CNeuronPool>())
    {
        std::ostringstream os;
        os << log_root << "layer_" << owner.m_tag;
        m_log.open(os.str());
    }
    
    ~CPrivate() final = default;
    void Motivate(std::unique_ptr<CEmotion>); // deprecated
    void Motivate_v2(std::unique_ptr<CEmotion>);
    std::ofstream& log()
    {
        m_log << "[layer_"<< m_owner.m_tag <<"]";
        return m_log;
    }
    
    bool isAlive() const
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
	CLayer& nextlayer();
    
	// emotion transfer
	void Transfer(std::unique_ptr<CEmotion> e);
	std::function<void(std::unique_ptr<CEmotion>)> m_fnTransEmotion;
	
	// associated neuron biulding helper
    std::set<size_t> m_tagUndupped;
    CTagIndex m_tagIdx;
    std::map<std::shared_ptr<CTagIndex>, std::shared_ptr<CNeuron> , CSptrLess<CTagIndex> > m_idxCache;
    std::vector< std::shared_ptr< CNeuron > > m_vecUndupNeuron;
    void buildAssoiatedNeuron(std::shared_ptr<decltype(m_vecUndupNeuron)>); // deprecated
    std::shared_ptr<CTagIndexChecker> m_spChecker; // deprecated
    std::shared_ptr<CNeuronPool> m_spNeurPool;
    
    // log
    std::ofstream m_log;
};

CLayerWork::CLayerWork(ILifeCycle& lc, CLayer& owner, CLayerGenerator& gen, std::function<void(std::unique_ptr<CEmotion>)> fnTrans)
: mp(std::make_shared<CPrivate>(lc, owner, gen, fnTrans))
{
}

void CLayerWork::operator()()
{
	while (mp->isAlive()) {
        auto e = mp->getEmotion();
        if (e)
        {
            mp->Motivate_v2(std::move(e));
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

CLayer* CLayerGenerator::getNewLayer(ILifeCycle& lc, std::function<void(std::unique_ptr<CEmotion>)> fnTrans)
{
    std::lock_guard<std::mutex> _(m_mutex);
    auto spLayer = std::make_shared<CLayer>(m_vecLayers.size()); // the vector size as layer tag
    m_vecLayers.push_back(spLayer);
    
    auto spWork = std::make_shared<CLayerWork>(lc, *spLayer, *this, fnTrans);
    m_vecWork.push_back(spWork);
    *spLayer = std::thread(*spWork);
    
    return spLayer.get();
}

inline CLayer & CLayerWork::CPrivate::nextlayer() {
	if (!m_pNextLayer) {
		m_pNextLayer = m_gen.getNewLayer(m_lc, m_fnTransEmotion);
	}
	return *m_pNextLayer;
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

// deprecated
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

void CLayerWork::CPrivate::Motivate_v2(std::unique_ptr<CEmotion> e)
{
    log() << "emotion tag " << e->echo() << std::endl;
    
    auto curNeur = m_spNeurPool->buildNeuron(e->m_tag);
    curNeur->strengthen();
    
	auto tagTrans = e->m_tag;
	bool ok, flagTrans = true;
    std::tie(std::ignore, ok) = m_tagUndupped.insert(e->m_tag);
    if (!ok)
    {   // something dupped and clean the undupped tags
        m_tagUndupped.clear();
        
        auto spIdx = std::make_shared<CTagIndex>();
        spIdx->Swap(m_tagIdx); // clean the tag sequence
        
        auto spUndupNeur = std::make_shared<decltype(m_vecUndupNeuron)>();
        spUndupNeur->swap(m_vecUndupNeuron);
        if (spIdx->Size() > 1) {
            decltype(m_idxCache)::iterator it;
            std::tie(it, ok) = m_idxCache.emplace(spIdx, nullptr);
            if (ok) {
                auto newNeur = m_spNeurPool->buildNeuron(CEmotion::getUniqueTag());
                auto spDendrite = newNeur->buildDendrite(newNeur);
                std::for_each(spUndupNeur->begin(), spUndupNeur->end(),[spDendrite](std::shared_ptr<CNeuron> spN) {
                    auto spAxon = spN->buildAxon(spN);
                    spDendrite->attach(spAxon);
                    spAxon->attach(spDendrite);
                });
                
                it->second = newNeur;
            }else{
                // next layer sending
                nextlayer().Send(std::make_unique<CEmotion>(it->second->m_tag));
				flagTrans = false;
            }

			tagTrans = it->second->m_tag;
		}
		else if(spIdx->Size() == 1)
		{
			// actually the tagTrans is same with e->m_tag
			tagTrans = spIdx->getSingleOne();
		}
        
        m_tagUndupped.insert(e->m_tag);
	}
	else
	{
		flagTrans = false;
	}

	if (flagTrans) {
		Transfer(std::make_unique<CEmotion>(tagTrans));
	}

    m_tagIdx.Add(e->m_tag);
    m_vecUndupNeuron.push_back(curNeur);
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

void CLayerWork::CPrivate::Transfer(std::unique_ptr<CEmotion> e)
{
	if (m_fnTransEmotion)
	{
		m_fnTransEmotion(std::move(e));
	}
}
