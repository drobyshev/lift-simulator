#include "Lift.h"
#include <regex>

namespace lift_simulator
{
Lift::Lift(const LiftConfig& config)
    : m_config(config)
    , m_isDone(false)
    , m_liftIsEmpty(true)
    , m_lastFloorNum(1)
    , m_liftHandler(*this)
{}

Lift::~Lift()
{
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

void Lift::Run()
{
    m_streamManager.Run();
    RunWorker();
    ReadCommands();
}

void Lift::RunWorker()
{
    m_worker = std::thread([this]()
    {
        std::unique_lock<std::mutex> locker(m_conditionWorkerMutex);
        while (!m_isDone)
        {
            m_conditionWorker.wait(locker, [this]() { return !IsEmptyEventsQueue() || m_isDone.load(); });
            if (m_isDone)
            {
                return;
            }
            while (!IsEmptyEventsQueue())
            {
                if (m_isDone)
                {
                    return;
                }
                if (const auto event = PopEvent())
                {
                    event->Apply(m_liftHandler);
                }
            }
        }
    });
}

void Lift::ReadCommands()
{
    while (!m_isDone)
    {
        const std::string buffer = m_streamManager.GetLine();
        if (buffer == "done")
        {
            return Done();
        }
        else if (std::regex_match(buffer, std::regex("goto\\s\\d+")))
        {
            if (m_liftIsEmpty)
            {
                m_streamManager.Print("Warning! Lift is empty. You can not press floor button inside lift");
                continue;
            }
            ParseCommand(buffer);
        }
        else if (std::regex_match(buffer, std::regex("call\\s\\d+")))
        {
            ParseCommand(buffer);
        }
        else
        {
            m_streamManager.Print("Warning! Wrong command");
        }
    }
}

void Lift::ParseCommand(const std::string& command)
{
    const std::regex floorNumRegex("\\d+");
    const auto it = std::sregex_iterator(command.begin(), command.end(), floorNumRegex);
    if (it != std::sregex_iterator())
    {
        const int floorNum = std::atoi(it->str().c_str());
        if (!ValidateFloorNum(floorNum, m_config.floorsCount))
        {
            std::stringstream ss;
            ss << "Warning! Wrong floor number: " << floorNum;
            m_streamManager.Print(ss.str());
            return;
        }
        MakeEvents(floorNum);
        m_conditionWorker.notify_one();
    }
}

bool Lift::IsEmptyEventsQueue()
{
    const std::lock_guard<std::mutex> locker(m_eventsMutex);
    return m_eventsQueue.empty();
}

Lift::LiftEventPtr Lift::PopEvent()
{
    const std::lock_guard<std::mutex> locker(m_eventsMutex);
    if (m_eventsQueue.empty())
    {
        return LiftEventPtr();
    }
    const LiftEventPtr event = m_eventsQueue.front();
    m_eventsQueue.pop_front();
    return event;
}

void Lift::MakeEvents(int floorNum)
{
    const std::lock_guard<std::mutex> locker(m_eventsMutex);
    auto it = std::find_if(m_eventsQueue.begin(), m_eventsQueue.end(), [floorNum](const auto& event)
    {
        return event ? event->GetFloorNum() == floorNum : false;
    });

    if (it != m_eventsQueue.end())
    {
        const auto nextIt = std::next(it);
        if (nextIt != m_eventsQueue.end() && (*nextIt)->GetFloorNum() != floorNum)
        {
            *it = std::make_shared<TargetFloorEvent>(floorNum);
            m_eventsQueue.insert(nextIt, std::make_shared<PassFloorEvent>(floorNum));
        }
        return;
    }

    if (m_lastFloorNum < floorNum)
    {
        for (int i = m_lastFloorNum; i < floorNum; ++i)
        {
            m_eventsQueue.push_back(std::make_shared<PassFloorEvent>(i));
        }
    }
    else if (m_lastFloorNum > floorNum)
    {
        for (int i = m_lastFloorNum; i > floorNum; --i)
        {
            m_eventsQueue.push_back(std::make_shared<PassFloorEvent>(i));
        }
    }
    m_lastFloorNum = floorNum;
    m_eventsQueue.push_back(std::make_shared<TargetFloorEvent>(floorNum));
}

void Lift::TargetFloor(int floorNum)
{
    std::stringstream ss;
    ss << "Floor #" << floorNum << ", open doors";
    PrintWithDelay(ss.str(), m_config.timeoutDoors);

    if (m_isDone)
    {
        return;
    }

    ss.str(std::string());
    ss << "Floor #" << floorNum << ", close doors";
    PrintWithDelay(ss.str(), m_config.timeoutDoors);

    m_liftIsEmpty = false;
}

void Lift::PassFloor(int floorNum)
{
    std::stringstream ss;
    ss << "Floor #" << floorNum << ", passed";
    PrintWithDelay(ss.str(), m_config.timeoutFloors);
}

void Lift::Done()
{
    m_isDone = true;
    m_streamManager.Done();
    m_conditionWorker.notify_one();
    m_conditionDelay.notify_one();
}

void Lift::PrintWithDelay(const std::string& msg, int delay)
{
    std::unique_lock<std::mutex> locker(m_conditionDelayMutex);
    m_conditionDelay.wait_for(locker, std::chrono::milliseconds(delay), [this]() { return m_isDone.load(); });
    if (!m_isDone)
    {
        m_streamManager.Print(msg);
    }
}
}
