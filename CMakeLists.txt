cmake_minimum_required(VERSION 2.8)
project(JniHelpers)

if(NOT ANDROID)
  find_package(JNI REQUIRED)
endif(NOT ANDROID)
include_directories(${JNI_INCLUDE_DIRS})

add_subdirectory(src/main/cpp)
add_subdirectory(src/test/cpp)
add_subdirectory(src/test/data)
