#ifndef LIFT_H
#define LIFT_H

#include <list>

#include "ILift.h"
#include "LiftConfig.h"
#include "LiftEvents.h"
#include "StreamManager.h"

namespace lift_simulator
{
class Lift : public ILift
{
public:
    Lift(const LiftConfig& config);

    ~Lift();

    void TargetFloor(int floorNum) override;

    void PassFloor(int floorNum) override;

    void Run();

private:
    using LiftEventPtr = std::shared_ptr<ILiftEvent>;

    void Done();

    bool IsEmptyEventsQueue();

    LiftEventPtr PopEvent();

    void MakeEvents(int floorNum);

    void ParseCommand(const std::string& buffer);

    void RunWorker();

    void ReadCommands();

    void PrintWithDelay(const std::string& msg, int delay);

    std::list<LiftEventPtr>  m_eventsQueue;
    LiftConfig               m_config;
    std::atomic<bool>        m_isDone;
    std::atomic<bool>        m_liftIsEmpty;
    int                      m_lastFloorNum;
    LiftHandler              m_liftHandler;
    std::thread              m_worker;
    std::mutex               m_conditionWorkerMutex;
    std::mutex               m_eventsMutex;
    std::mutex               m_conditionDelayMutex;
    std::condition_variable  m_conditionWorker;
    std::condition_variable  m_conditionDelay;
    StreamManager            m_streamManager;
};
}

#endif // LIFT_H
