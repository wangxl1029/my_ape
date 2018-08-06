

/// follow the mac header style 
#ifndef ai_common_
#define ai_common_

#ifdef __APPLE__
/* The classes below are exported */
#pragma GCC visibility push(default)
#endif // __APPLE__

#ifdef __APPLE__
#   include "TargetConditionals.h"
#   ifdef TARGET_OS_MAC
#       include <cstdlib> // size_t
#   else
#       error "error environment"
#   endif
#elif defined(_WIN32)
#	include <limits>
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
#ifdef __APPLE__
#pragma GCC visibility pop
#endif // __APPLE__

#endif
