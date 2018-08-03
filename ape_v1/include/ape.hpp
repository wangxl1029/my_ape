#pragma once

#include "ai_comm.hpp"
#include <memory>

#ifndef ape_lib_
#define ape_lib_

/* The classes below are exported */
#pragma GCC visibility push(default)

namespace nsAI
{
    class CBrain;
    class CSpine;
    class CInput;
    class COutput;
    
    class CApe : public nsAI::CNoCopyable
    {
    public:
        CApe();
        ~CApe() final = default;
        
        void read(const char*);
        void kill();
    private:
#if 0
        CBrain    m_brain;
        CInput    m_input;
        COutput    m_output;
        nsNeuronal::CBusServer m_NeurBus;
#else
        std::shared_ptr<CBrain> m_spBrain;
        std::shared_ptr<CSpine> m_spSpine;
        std::shared_ptr<CInput> m_spInput;
        std::shared_ptr<COutput> m_spOutput;
#endif
    };
    
}


#pragma GCC visibility pop
#endif

















