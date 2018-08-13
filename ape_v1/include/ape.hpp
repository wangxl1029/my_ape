#ifndef ape_lib_
#define ape_lib_

#ifdef __APPLE__
/* The classes below are exported */
#pragma GCC visibility push(default)
#endif // __APPLE__

namespace nsAI
{
    class CApe : public nsAI::CNoCopyable
    {
        class CBrain;
        class CSpine;
        class CInput;
        class COutput;
    public:
        CApe();
        ~CApe() final = default;
        
        void read(const char*);
        void kill();
    private:
        std::shared_ptr<CBrain> m_spBrain;
        std::shared_ptr<CSpine> m_spSpine;
        std::shared_ptr<CInput> m_spInput;
        std::shared_ptr<COutput> m_spOutput;
    };
}

#ifdef __APPLE__
#pragma GCC visibility pop
#endif // __APPLE__

#endif

















