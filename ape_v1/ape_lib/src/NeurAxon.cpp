#include <cassert>
#include <memory>
#include <vector>

#include "ai_comm.hpp"
#include "ai_access_imp.hpp"
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"

namespace nsAI
{
	namespace nsNeuronal
	{
		CAxon::CAxon(std::shared_ptr<CNeuron> spOwner)
			: m_spOwner(spOwner)
			, m_spDendrite(nullptr)
		{
		}

		void CAxon::attach(std::shared_ptr<CDendrite> spDen)
		{
			assert(spDen);
			m_spDendrite = spDen;
		}

		std::shared_ptr<CDendrite> CAxon::getDendrite() const
		{
			return m_spDendrite;
		}

		std::shared_ptr<CNeuron> CAxon::getOwner() const
		{
			return m_spOwner;
		}
	}
}
