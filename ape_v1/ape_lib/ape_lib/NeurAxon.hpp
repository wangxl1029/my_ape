#ifndef _neuronal_axon_hpp_
#define _neuronal_axon_hpp_

//#include <memory>
//#include "NeuronDef.hpp"

namespace nsAI
{
	namespace nsNeuronal
	{
		class CDendrite;
        class CNeuron;
		class CAxon : public CObject
		{
		public:
            CAxon() = delete;
            CAxon(std::shared_ptr<CNeuron> spOwner);
			~CAxon() final = default;
			void attach(std::shared_ptr<CDendrite>);
            std::shared_ptr<CDendrite> getDendrite() const;
            std::shared_ptr<CNeuron> getOwner() const;
		private:
			std::shared_ptr<CDendrite> m_spDendrite;
            std::shared_ptr<CNeuron> m_spOwner;
		};
	}
}
#endif
