//
//  BusMessage.cpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//
#include <cassert>
#include <iostream>
#include "BusMessage.hpp"

namespace ns = nsAI::nsNeuronal::nsBus;
    
nsAI::nsNeuronal::nsBus::CMsgText::CData::CData(const char* s)
{
    assert(s);
    m_text = s;
}
    
std::unique_ptr<ns::CMsgText::CData> ns::CMsgText::getDataUniquePtr(std::unique_ptr<nsAI::CObject> upData)
{
    if (upData)
    {
        auto p = dynamic_cast<CData*>( upData.get() );
        if (p)
        {
            upData.release();
            return std::unique_ptr<CData>(p);
        }
        else
        {
            std::cerr << "base/derived not matched" << std::endl;
        }
    }
    return nullptr;
}

