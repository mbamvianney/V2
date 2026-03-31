cmake_minimum_required(VERSION 3.16)
project(StudentSplitBenchmark VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(student_split ${SOURCES})
