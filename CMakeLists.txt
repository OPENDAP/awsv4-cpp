cmake_minimum_required(VERSION 3.15)
project(awsv4_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(/Users/jimg/src/opendap/hyrax_git/build/deps/include)
include_directories(/usr/local/Cellar/openssl@1.1/1.1.1d/include)

add_executable(awsv4_cpp
		awsv4.cc
		awsv4.h
		main.cc)
