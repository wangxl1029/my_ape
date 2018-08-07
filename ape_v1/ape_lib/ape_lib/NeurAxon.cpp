#include <cassert>
#include "NeurAxon.hpp"

namespace nsAI
{
	namespace nsNeuronal
	{
		inline CAxon::CAxon() : m_pDendrite(nullptr)
		{
		}
		inline void CAxon::attach(CDendrite * pDen)
		{
			assert(pDen);
			m_pDendrite = pDen;
		}
	}
}