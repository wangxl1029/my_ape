#pragma once
#include <memory>
#include <vector>
#include "ai_comm.hpp"

namespace nsAI {
	namespace nsNeuronal {
		class CAxon;
		class CDendrite : public CObject
		{
		public: // ctor/dtor
			CDendrite() = default;
			~CDendrite() final = default;
			// property
			size_t getAxonNum() const;
			// modifier
			void attach(std::shared_ptr<CAxon> spAxon);
		private:
			std::vector< std::shared_ptr<CAxon> > m_vecAxon;
		};

	}
}