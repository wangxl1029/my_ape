//
//  NeurUnconsci.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/4.
//  Copyright © 2018 alan king. All rights reserved.
//

#include <iostream>

#include "EmotionTarget.hpp"
#include "BusClient.hpp"
#include "NeurUnconsci.hpp"

#include "NeuronalLayerPool.hpp"

namespace nsAI {namespace nsNeuronal{
}} // enclosed nsAI::nsNeuronal


using namespace nsAI::nsNeuronal;

void CReflect::operator()()
{
	CLayerPool layerPool;
	while (m_pCortex->isAlive())
	{
		auto e = m_pUnconsci->getEmotion();
		if (e)
		{
			if (e->m_tag < CEmotion::EMOTION_E_MAX)
			{
				auto tag = static_cast<CEmotion_E>(e->m_tag);
				switch (tag)
				{
				case nsAI::nsNeuronal::CEmotion_E::input_absence:
					break;
				case nsAI::nsNeuronal::CEmotion_E::input_test:
					break;
				case nsAI::nsNeuronal::CEmotion_E::input_txt:
					break;
				case nsAI::nsNeuronal::CEmotion_E::instinct_crying:
					break;
				case nsAI::nsNeuronal::CEmotion_E::instinct_sleep:
					break;
				case nsAI::nsNeuronal::CEmotion_E::instinct_idle:
					break;
				case nsAI::nsNeuronal::CEmotion_E::max:
					break;
				default:
					break;
				}
			}

			//e = triggerReflexUnconsciously(std::move(e));

			//m_pMind->Send(std::move(e));
#if 1
            std::cout << "sensor: " << e->echo() << std::endl;
            layerPool.Send(std::move(e));
#else
            if (static_cast<size_t>(nsAI::nsNeuronal::CEmotion_E::input_txt) == e->m_tag) {
                std::cout << "sensor: " << CEmotion::echo(e->m_tag) << " hint : " << e->m_hint << std::endl;
                layerPool.Send(std::make_unique<CEmotion>(e->m_hint));
            }else{
                std::cout << "sensor: " << CEmotion::echo(e->m_tag) << std::endl;
                layerPool.Send(std::move(e));
            }
#endif
		}
	}
    layerPool.Kill();
}
