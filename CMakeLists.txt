cmake_minimum_required(VERSION 3.31)
project(CourseRegistrationSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(CourseRegistrationSystem main.cpp
        Models.h
        DataStructures.h
        System.h
        System.cpp)
