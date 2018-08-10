#include <cassert>
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
namespace nsAI {
	namespace nsNeuronal {
		size_t CDendrite::getAxonNum() const
		{
			return m_vecAxon.size();
		}

        void CDendrite::attach(std::shared_ptr<CAxon> spAxon)
		{
            m_vecAxon.push_back(spAxon);
		}
	}
}
