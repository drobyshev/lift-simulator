#include "StreamManager.h"

#include <termios.h>
#include <unistd.h>
#include <iostream>

namespace
{
// stolen from the web
char getch() {
        char buf = 0;
        struct termios old = {};
        if (tcgetattr(0, &old) < 0)
                perror("");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("");
        return (buf);
}
}

namespace lift_simulator
{
StreamManager::StreamManager()
    : m_isDone(false)
    , m_isNotified(false)
{}

StreamManager::~StreamManager()
{
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

void StreamManager::Run()
{
    m_worker = std::thread([this]()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (!m_isDone)
        {
            m_conditionWorker.wait(locker, [this]() { return m_isNotified.load(); });
            std::cout << "\33[2K\r";
            if (m_isDone)
            {
                return;
            }
            if (!m_outputBuf.empty())
            {
                std::cout << m_outputBuf << std::endl;
                m_outputBuf.clear();
            }
            std::cout << "> " << m_inputBuf << std::flush;
            m_isNotified = false;
        }
    });
    Print(std::string());
}

std::string StreamManager::GetLine()
{
    const char BACKSPACE = 127;
    while (const char ch = getch())
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (BACKSPACE == ch)
        {
            if (!m_inputBuf.empty())
            {
                m_inputBuf.pop_back();
            }
            Notify();
            continue;
        }
        if (ch == '\n')
        {
            const std::string buffer = m_inputBuf;
            m_inputBuf.clear();
            Notify();
            return buffer;
        }
        m_inputBuf.push_back(ch);
        Notify();
    }
    return std::string();
}

void StreamManager::Print(const std::string& str)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_outputBuf += str;
    Notify();
}

void StreamManager::Done()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_isDone = true;
    Notify();
}

void StreamManager::Notify()
{
    m_isNotified = true;
    m_conditionWorker.notify_one();
}
}
