#include <cassert>
#include "NeurAxon.hpp"
#include "NeurDendrite.hpp"
#include "NeuronDef.hpp"
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
	}
}
