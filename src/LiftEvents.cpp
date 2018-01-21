#include "LiftEvents.h"

namespace lift_simulator
{
TargetFloorEvent::TargetFloorEvent(int targetFloor)
    : m_targetFloor(targetFloor)
{}

void TargetFloorEvent::Apply(ILiftHandler& handler)
{
    handler.Handle(*this);
}

int TargetFloorEvent::GetFloorNum() const
{
    return m_targetFloor;
}

PassFloorEvent::PassFloorEvent(int passedFloor)
    : m_passedFloor(passedFloor)
{}

void PassFloorEvent::Apply(ILiftHandler& handler)
{
    handler.Handle(*this);
}

int PassFloorEvent::GetFloorNum() const
{
    return m_passedFloor;
}

LiftHandler::LiftHandler(ILift& lift)
    : m_lift(lift)
{}

void LiftHandler::Handle(const TargetFloorEvent& event)
{
    m_lift.TargetFloor(event.GetFloorNum());
}

void LiftHandler::Handle(const PassFloorEvent& event)
{
    m_lift.PassFloor(event.GetFloorNum());
}
}
