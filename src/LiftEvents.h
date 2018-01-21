#ifndef LIFT_EVENTS_H
#define LIFT_EVENTS_H

#include "ILift.h"

class ILiftHandler;

namespace lift_simulator
{
class ILiftHandler;

class ILiftEvent
{
public:
    virtual void Apply(ILiftHandler& handler) = 0;

    virtual int GetFloorNum() const = 0;

    virtual ~ILiftEvent() {}
};

class TargetFloorEvent : public ILiftEvent
{
public:
    explicit TargetFloorEvent(int targetFloor);

    void Apply(ILiftHandler& handler) override;

    int GetFloorNum() const override;

private:
    const int m_targetFloor;
};

class PassFloorEvent : public ILiftEvent
{
public:
    explicit PassFloorEvent(int passedFloor);

    void Apply(ILiftHandler& handler) override;

    int GetFloorNum() const override;

private:
    const int m_passedFloor;
};

class ILiftHandler
{
public:
    virtual void Handle(const TargetFloorEvent& event) = 0;

    virtual void Handle(const PassFloorEvent& event) = 0;

    virtual ~ILiftHandler() {}
};

class LiftHandler : public ILiftHandler
{
public:
    explicit LiftHandler(ILift& lift);

    void Handle(const TargetFloorEvent& event) override;

    void Handle(const PassFloorEvent& event) override;

private:
    ILift& m_lift;
};
}

#endif // LIFT_EVENTS_H
