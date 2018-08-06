//
//  BusServer.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef BusServer_hpp
#define BusServer_hpp

#include "ai_priv.hpp"
#include "BusMessage.hpp"
namespace nsAI{
    namespace nsNeuronal{
        namespace nsBus{
            typedef CActiveTarget<CMessage> CMessageTarget_t;
        }
    } /// nsNeuronal
}; ///< nsAI
                
#endif /* BusServer_hpp */
