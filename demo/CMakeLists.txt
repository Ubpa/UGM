# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(demo_project VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(UGM REQUIRED)

add_executable(demo main.cpp)

target_link_libraries(demo PUBLIC Ubpa::UGM_core)
