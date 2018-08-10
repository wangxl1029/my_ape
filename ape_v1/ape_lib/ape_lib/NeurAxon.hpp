#ifndef _neuronal_axon_hpp_
#define _neuronal_axon_hpp_

#include <memory>
#include "ai_comm.hpp"

namespace nsAI
{
	namespace nsNeuronal
	{
		class CDendrite;
		class CAxon : public CObject
		{
		public:
			CAxon();
			~CAxon() final = default;
			void attach(std::shared_ptr<CDendrite>);
		private:
			std::shared_ptr<CDendrite> m_pDendrite;
		};


	}
}
#endif
