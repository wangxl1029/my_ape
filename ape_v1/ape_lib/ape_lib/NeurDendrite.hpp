#pragma once
#include <memory>
#include <vector>
#include "ai_comm.hpp"

namespace nsAI {
	namespace nsNeuronal {
		class CAxon;
        class CNeuron;
		class CDendrite : public CObject
		{
		public: // ctor/dtor
			CDendrite() = delete;
            CDendrite(std::shared_ptr<CNeuron> spOwner);
			~CDendrite() final = default;
			// property
			size_t getAxonNum() const;
			// modifier
			void attach(std::shared_ptr<CAxon> spAxon);
		private:
            std::shared_ptr<CNeuron> m_spOwner;
			std::vector< std::shared_ptr<CAxon> > m_vecAxon;
        public:
            // accessor
            std::shared_ptr<CNeuron> getOwner() const;
		};

	}
}
