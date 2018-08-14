//
//  IAccessor.h
//  ape_lib
//
//  Created by alan king on 2018/8/11.
//  Copyright © 2018 alan king. All rights reserved.
//

#ifndef IAccessor_h
#define IAccessor_h

namespace nsAI {
    
    template<class _T>
    class IAccessor : public CNoCopyable
    {
    public:
        ~IAccessor() override = default;
        virtual typename _T::size_type getSize() const = 0;
        virtual std::unique_ptr<CNoCopyable> getFirst() = 0;
        virtual typename _T::value_type getNext(CNoCopyable*) = 0;
        virtual bool isEnded(CNoCopyable*) const = 0;
        virtual void reset(CNoCopyable*) = 0;
    };
}

#endif /* IAccessor_h */
