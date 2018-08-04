

/// follow the mac header style 
#ifndef ai_common_
#define ai_common_

/* The classes below are exported */
#pragma GCC visibility push(default)

#ifdef __APPLE__
#   include "TargetConditionals.h"
#   ifdef TARGET_OS_MAC
#       include <cstdlib> // size_t
#   else
#       error "error environment"
#   endif
#endif

namespace nsAI
{
    class CObject {
    public:
        virtual ~CObject() = default;
    };
    
    class CNoCopyable : public CObject
    {
    public:
        CNoCopyable() = default;
        CNoCopyable(const CNoCopyable&) = delete;
        CNoCopyable& operator=(const CNoCopyable&) = delete;
    };
    
}

#pragma GCC visibility pop
#endif
