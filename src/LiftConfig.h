#ifndef LIFT_CONFIG_H
#define LIFT_CONFIG_H

#include <experimental/optional>

namespace lift_simulator
{
struct LiftConfig
{
    int floorsCount = 0;
    int timeoutFloors = 0;
    int timeoutDoors = 0;
};

using LiftConfigOpt = std::experimental::optional<LiftConfig>;

LiftConfigOpt ReadLiftConfig(const std::string& floorsCountStr, const std::string& heightStr, const std::string& velocityStr,
                             const std::string& timeoutDoorsStr);

bool ValidateFloorNum(int floorNum, int floorCountMax);
}

#endif // LIFT_CONFIG_H
