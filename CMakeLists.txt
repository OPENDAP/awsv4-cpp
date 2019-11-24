cmake_minimum_required(VERSION 3.15)
project(awsv4_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(/Users/jimg/src/opendap/hyrax_git/duild/deps/include)
include_directories(-I/usr/local/Cellar/openssl@1.1/1.1.1d/lib)

add_executable(awsv4_cpp
        awsv4.cpp
        awsv4.hpp
        main.cpp)
