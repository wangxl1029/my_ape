#include <cassert>
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"
#include "CAccessor.hpp"
namespace nsAI {
	namespace nsNeuronal {
		CDendrite::CDendrite(std::shared_ptr<CNeuron> spOwner)
		{
			m_spOwner = spOwner;
		}

		size_t CDendrite::getAxonNum() const
		{
			return m_vecAxon.size();
		}

		void CDendrite::attach(std::shared_ptr<CAxon> spAxon)
		{
			m_vecAxon.push_back(spAxon);
		}

		std::shared_ptr<CNeuron> CDendrite::getOwner() const
		{
			return m_spOwner;
		}

		std::unique_ptr<CDendrite::AxonAccessor_t> CDendrite::getAxonAccessor()
		{
			return std::make_unique< CAccessor< decltype(m_vecAxon) > >(m_vecAxon.begin(), m_vecAxon.end(), m_vecAxon.size());
		}
	}
}
