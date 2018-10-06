cmake_minimum_required(VERSION 3.12)
project(fixed_vector)

set(CMAKE_CXX_STANDARD 14)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(fixed_vector_testing
        fixed_vector_testing.cpp
        fixed_vector.h
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -D_GLIBCXX")
endif()

target_link_libraries(fixed_vector_testing -lpthread)