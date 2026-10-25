cmake_minimum_required(VERSION 3.10)
project(MapNavigationSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(map_navigator
    src/main.cpp
    src/Graph.cpp
    src/NavigationSystem.cpp
)
