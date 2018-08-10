#include <cassert>
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"

namespace nsAI
{
	namespace nsNeuronal
	{
        CAxon::CAxon() : m_pDendrite(nullptr)
		{
		}
        void CAxon::attach(std::shared_ptr<CDendrite> spDen)
		{
			assert(spDen);
			m_pDendrite = spDen;
		}
	}
}
