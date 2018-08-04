//
//  BusServer.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusServer_hpp
#define BusServer_hpp

#include "IBusServer.hpp"
namespace nsAI{
    namespace nsNeuronal{

        class CBusServer : protected CActiveTarget<nsBus::CMessage>, public IBusServer
        {
        public:
            
            CBusServer() = default;
            ~CBusServer() override = default;
            
            void Send(std::unique_ptr<nsBus::CMessage> m) final;
            void Connect(CConnectiveTarget_E type, CActiveTarget* target) final;

            
            void Kill() final
            {
            }
            
        private:
            std::atomic< CActiveTarget*> m_pInput;
            std::atomic< CActiveTarget*> m_pOutput;
            std::atomic< CActiveTarget*> m_pCortex;
        };

    } /// nsNeuronal
}; ///< nsAI
                
#endif /* BusServer_hpp */
