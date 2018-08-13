#pragma once
//#include "NeurDendrite.hpp"
//#include "NeurAxon.hpp"
//#include "IAccessor.hpp"
namespace nsAI {
	namespace nsNeuronal {
        class CAxon;
        class CDendrite;
		class CNeuron : public CObject
		{
		public:
			CNeuron();
			CNeuron(size_t tag);
			~CNeuron() override = default;
			size_t strengthen() { return m_strongVal++; }
			std::shared_ptr< CDendrite > buildDendrite(std::shared_ptr<CNeuron> spOwner);
			std::shared_ptr< CAxon > buildAxon(std::shared_ptr<CNeuron> spOwner);
            // prediction
			struct SPtrLess {
				bool operator()(std::shared_ptr<CNeuron>, std::shared_ptr<CNeuron>) const;
			};
			static bool less(const CNeuron&, const CNeuron&);
			static bool less(std::shared_ptr<CNeuron>, std::shared_ptr<CNeuron>);
            // attributes
			const size_t m_tag;
		private:
			size_t m_strongVal;
			std::vector< std::shared_ptr< CAxon > > m_vecAxon;
			std::vector< std::shared_ptr< CDendrite > > m_vecDendrite;
        public:
            // accessor
            typedef IAccessor< decltype(m_vecAxon) > AxonAccessor_t;
            typedef IAccessor< decltype(m_vecDendrite) > DendriAccessor_t;
            std::unique_ptr<AxonAccessor_t> getAxonAccessor();
            std::unique_ptr<DendriAccessor_t> getDendriAccessor();
		};

	}
}
