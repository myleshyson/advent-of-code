cmake_minimum_required(VERSION 3.29)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")

find_package(benchmark REQUIRED)

include_directories(/usr/local/include)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        message("Configuring for Release")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Release build flags" FORCE)
endif()

add_executable(question_1 1.cpp
        helpers.h
        helpers.cpp
        )
add_executable(question_2 2.cpp
        helpers.h
        helpers.cpp
)
add_executable(question_3 3.cpp helpers.h helpers.cpp)
add_executable(question_4 4.cpp helpers.cpp helpers.h)
add_executable(question_5 5.cpp helpers.cpp helpers.h)
add_executable(question_6 6.cpp helpers.cpp helpers.h)
add_executable(question_7 7.cpp helpers.cpp helpers.h)
add_executable(question_8 8.cpp helpers.cpp helpers.h)
add_executable(question_9 9.cpp helpers.cpp helpers.h)
add_executable(question_10 10.cpp helpers.h helpers.cpp)
add_executable(question_11 11.cpp helpers.cpp helpers.h)
add_executable(question_12 12.cpp helpers.cpp helpers.h)
add_executable(question_13 13.cpp helpers.h helpers.cpp)
add_executable(question_14 14.cpp helpers.h helpers.cpp)
add_executable(question_15 15.cpp helpers.h helpers.cpp)
add_executable(question_16 16.cpp helpers.h helpers.cpp)
add_executable(question_17 17.cpp helpers.cpp helpers.h)
add_executable(question_18 18.cpp helpers.cpp helpers.h)
add_executable(question_19 19.cpp helpers.cpp helpers.h)
add_executable(question_20 20.cpp helpers.h helpers.cpp)

target_link_libraries(question_11 benchmark::benchmark)
target_link_libraries(question_12 benchmark::benchmark)
