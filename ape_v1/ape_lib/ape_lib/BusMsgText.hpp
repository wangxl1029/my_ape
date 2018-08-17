//
//  BusMsgText.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/17.
//  Copyright © 2018年 alan king. All rights reserved.
//

#ifndef BusMsgText_hpp
#define BusMsgText_hpp

namespace nsAI {namespace nsNeuronal{namespace nsBus{
    class CMsgText : public CMessage
    {
    public:
        class CData : public CObject
        {
        public:
            CData(const char* s); // s must NOT be nullptr
            ~CData() final = default;
            size_t getCStrSize() const {return m_text.size();}
            char getCharAt(size_t pos) const {return m_text.at(pos);}
        private:
            std::string m_text;
        };
        
        CMsgText() : CMessage(CMessageId_E::CORTEX_TEXT_INPUT)
        {
        }
        
        CMsgText(const char* s) : CMessage(CMessageId_E::CORTEX_TEXT_INPUT)
        {
            if (s)
            {
                m_upData = std::make_unique<CData>(s);
            }
        }
        
        ~CMsgText() final = default;
        
        static std::unique_ptr<CMsgText> CreateUniquePtr(const char* s)
        {
            return s ? std::make_unique<nsNeuronal::nsBus::CMsgText>(s) : std::make_unique<nsNeuronal::nsBus::CMsgText>();
        }
        
        static std::unique_ptr<CData> getDataUniquePtr(std::unique_ptr<CObject> upData);
    };

}}}
#endif /* BusMsgText_hpp */
