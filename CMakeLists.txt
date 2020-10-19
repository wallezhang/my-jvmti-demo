cmake_minimum_required(VERSION 3.17)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
include_directories(/Library/Java/JavaVirtualMachines/jdk1.8.0_261.jdk/Contents/Home/include)
include_directories(/Library/Java/JavaVirtualMachines/jdk1.8.0_261.jdk/Contents/Home/include/darwin)
add_library(untitled SHARED jvmti_example.cpp)