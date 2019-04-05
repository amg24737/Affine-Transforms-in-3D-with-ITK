cmake_minimum_required(VERSION 3.13)
project(AffineTransforms)

set(CMAKE_CXX_STANDARD 14)

find_package(ITK)
include(${ITK_USE_FILE})

add_executable(AffineTransformsBuild hw2.cpp)

target_link_libraries(AffineTransformsBuild ${ITK_LIBRARIES})
