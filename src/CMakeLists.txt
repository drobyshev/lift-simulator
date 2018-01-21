project(Lift)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

set(SRC
    ILift.h
    LiftEvents.h
    LiftEvents.cpp
    LiftConfig.h
    LiftConfig.cpp
    Lift.h
    Lift.cpp
    StreamManager.h
    StreamManager.cpp
    Main.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Werror -Wpedantic)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
