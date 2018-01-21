#ifndef ILIFT_H
#define ILIFT_H

namespace lift_simulator
{
class ILift
{
public:
    virtual void TargetFloor(int floorNum) = 0;

    virtual void PassFloor(int floorNum) = 0;

    virtual ~ILift() {}
};
}

#endif // ILIFT_H
