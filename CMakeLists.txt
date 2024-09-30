cmake_minimum_required(VERSION 3.10)

# Set the project name based on the directory name
project(event)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("E:/team_assignment/course/Data Structures And Algorithms/bankqueue_init/include")

# Add the executable
add_executable(event "E:/team_assignment/course/Data Structures And Algorithms/bankqueue_init/src/event.cpp")