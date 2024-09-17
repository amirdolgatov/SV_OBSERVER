cmake_minimum_required(VERSION 3.26)
project(IEC61850_Observer)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
add_subdirectory(Test)
add_subdirectory(libs/googletest)

