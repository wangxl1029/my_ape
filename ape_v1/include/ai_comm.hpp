

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
#	ifndef SIZE_T_MAX
#	define SIZE_T_MAX std::numeric_limits<size_t>::max()
#	endif // SIZE_T_MAX
#endif

namespace nsAI
{
    class CObject {
    public:
        CObject() = default;
        virtual ~CObject() = default;
    };
    
    class CNoCopyable
    {
    public:
        CNoCopyable() = default;
        virtual ~CNoCopyable() = default;
        CNoCopyable(const CNoCopyable&) = delete;
        CNoCopyable& operator=(const CNoCopyable&) = delete;
    };
    
}
#ifdef __APPLE__
#pragma GCC visibility pop
#endif // __APPLE__

#endif
