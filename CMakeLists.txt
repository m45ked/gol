cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

add_executable(app
    main.cpp
    gol/mat.cpp
    gol/mat.hpp
    gol/StringFormatter.hpp
    gol/StringFormatter.cpp
)
