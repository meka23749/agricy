cmake_minimum_required(VERSION 3.16)

project(agricy)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

add_executable(agricy
    src/main.cpp
    src/SensorSimulator.cpp
    src/Database.cpp
)

target_include_directories(agricy PRIVATE
    include
    include/Crow/include
)

target_link_libraries(agricy PRIVATE
    SQLite::SQLite3
    Threads::Threads
)