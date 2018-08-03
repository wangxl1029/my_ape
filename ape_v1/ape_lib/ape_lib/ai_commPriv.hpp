//
//  ape_libPriv.hpp
//  ape_lib
//
//  Created by alan king on 2018/8/3.
//  Copyright © 2018年 alan king. All rights reserved.
//

/* The classes below are not exported */
#include <queue>
#include <thread>
#include <chrono>
#include "ai_comm.hpp"

#pragma GCC visibility push(hidden)

namespace nsAI
{
    

///< thread-safe queue template
template<class _T>
class CQueue_TS
{
public:
    CQueue_TS()
    {
    }
    
    ~CQueue_TS()
    {
    }
    
    std::unique_ptr<_T> Pop(size_t tmout_ms)
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        if (m_cv.wait_for(lk, std::chrono::milliseconds(tmout_ms), [this] {return !m_q.empty(); }))
        {
            auto p = std::move(m_q.front());
            m_q.pop();
            return p;
        }
        
        return nullptr;
    }
    
    void Push(std::unique_ptr<_T> p)
    {
        std::lock_guard<std::mutex> _(m_mutex);
        m_q.emplace(std::move(p));
        m_cv.notify_one();
    }
    
private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue< std::unique_ptr<_T> > m_q;
};

template<class _MsgT>
class CActiveTarget : public CNoCopyable
{
public:
    CActiveTarget()
    {
    }
    
    ~CActiveTarget() override
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    
    std::unique_ptr<_MsgT> PopMessage(size_t tmout_ms = 100)
    {
        return m_mq.Pop(tmout_ms);
    }
    
    void PushMessage(std::unique_ptr< _MsgT > m)
    {
        return m_mq.Push(std::move(m));
    }
    
    virtual void Kill() = 0;
protected:
    std::thread m_thread;
private:
    CQueue_TS<_MsgT> m_mq;
};
    
}; ///< nsAi
#pragma GCC visibility pop
