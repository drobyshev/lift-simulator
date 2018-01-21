#include <iostream>
#include "Lift.h"

int main(int argc, char* argv[])
{
    const int ARG_COUNT = 5;

    if (ARG_COUNT != argc)
    {
        std::cout << "Error! Wrong numbers argc: " << argc << std::endl;
        return 0;
    }

    if (const auto config = lift_simulator::ReadLiftConfig(argv[1], argv[2], argv[3], argv[4]))
    {
        lift_simulator::Lift lift(*config);
        lift.Run();
        return 0;
    }

    std::cout << "Error! Invalid config." << std::endl;
    return 0;
}
