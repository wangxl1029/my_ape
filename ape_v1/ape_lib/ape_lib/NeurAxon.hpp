#pragma once
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
			void attach(CDendrite*);
		private:
			CDendrite * m_pDendrite;
		};


	}
}