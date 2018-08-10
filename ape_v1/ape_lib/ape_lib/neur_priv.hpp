//
//  neur_priv.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/10.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef neur_priv_hpp
#define neur_priv_hpp

#include "ai_comm.hpp"

namespace nsAI {
    template<class _T>
    struct CCursor : public CNoCopyable
    {
        CCursor(typename _T::iterator it) : mIt(it) {}
        ~CCursor() final = default;
        typename _T::iterator mIt;
    };
    
}

#endif /* neur_priv_hpp */
