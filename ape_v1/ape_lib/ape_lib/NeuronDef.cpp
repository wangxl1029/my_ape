#include <algorithm>
#include <set>
#include <vector>

#include "ai_comm.hpp"
#include "CAccessor.hpp"
//#include "NeurEmotion.hpp"
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"

namespace nsAI {
	namespace nsNeuronal {



		inline CNeuron::CNeuron() :CNeuron(SIZE_T_MAX)
		{
		}

		CNeuron::CNeuron(size_t tag) : m_tag(tag), m_strongVal(0)
		{
		}

		std::shared_ptr<CDendrite> CNeuron::buildDendrite(std::shared_ptr<CNeuron> spOwner)
		{
			m_vecDendrite.push_back(std::make_shared<CDendrite>(spOwner));
			return m_vecDendrite.back();
		}

		std::shared_ptr<CAxon> CNeuron::buildAxon(std::shared_ptr<CNeuron> spOwner)
		{
			m_vecAxon.push_back(std::make_shared<CAxon>(spOwner));
			return m_vecAxon.back();
		}

		std::unique_ptr< CNeuron::AxonAccessor_t > CNeuron::getAxonAccessor()
		{
			return std::make_unique< CAccessor< decltype(m_vecAxon) > >(m_vecAxon.begin(), m_vecAxon.end(), m_vecAxon.size());
		}

		std::unique_ptr< CNeuron::DendriAccessor_t > CNeuron::getDendriAccessor()
		{
			return std::make_unique< CAccessor< decltype(m_vecDendrite) > >(m_vecDendrite.begin(), m_vecDendrite.end(), m_vecDendrite.size());
		}

	}
}
