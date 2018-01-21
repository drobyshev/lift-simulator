#include "LiftConfig.h"

#include <iostream>
#include <cmath>

namespace
{
const int MSEC_PER_SEC = 1000;
const int FLOOR_NUM_MIN = 1;
const int FLOOR_COUNT_MIN = 5;
const int FLOOR_COUNT_MAX = 20;
const double EPSILON = 1E-5;

bool ValidateFloor(int floorCount, int floorCountMin, int floorCountMax)
{
    return floorCount >= floorCountMin && floorCount <= floorCountMax;
}

bool FloatIsZero(double num)
{
    return std::abs(num) <= EPSILON;
}
}

namespace lift_simulator
{
LiftConfigOpt ReadLiftConfig(const std::string& floorsCountStr, const std::string& heightStr, const std::string& velocityStr,
                             const std::string& timeoutDoorsStr)
try
{
    const double height = std::stod(heightStr);
    const double velocity = std::stod(velocityStr);

    if (FloatIsZero(height) || FloatIsZero(velocity))
    {
        std::cout << "Error! Floor height or lift velocity is zero" << std::endl;
        return LiftConfigOpt();
    }

    const int timeoutFloors = static_cast<int>(height / velocity * MSEC_PER_SEC);
    const int timeoutDoors = std::stoi(timeoutDoorsStr) * MSEC_PER_SEC;
    const int floorsCount = std::stoi(floorsCountStr);

    return timeoutFloors >= 0 && timeoutDoors >= 0 && ValidateFloor(floorsCount, FLOOR_COUNT_MIN, FLOOR_COUNT_MAX) ?
        LiftConfigOpt(LiftConfig { floorsCount, timeoutFloors, timeoutDoors }) : LiftConfigOpt();
}
catch (const std::invalid_argument& ex)
{
    std::cout << "Error! " << ex.what() << std::endl;
    return LiftConfigOpt();
}

bool ValidateFloorNum(int floorNum, int floorCountMax)
{
    return ValidateFloor(floorNum, FLOOR_NUM_MIN, floorCountMax);
}
}
