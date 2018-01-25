#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

namespace lift_simulator
{
struct StreamManager
{
    StreamManager();

    ~StreamManager();

    void Run();

    void Print(const std::string& msg);

    std::string GetLine();

    void Done();

private:
    void Notify();

    std::string             m_inputBuf;
    std::string             m_outputBuf;
    std::thread             m_worker;
    std::mutex              m_mutex;
    std::atomic<bool>       m_isDone;
    std::atomic<bool>       m_isNotified;
    std::condition_variable m_conditionWorker;
};
}

#endif // STREAM_MANAGER_H

