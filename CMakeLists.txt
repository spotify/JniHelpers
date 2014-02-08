cmake_minimum_required(VERSION 2.8)
project(JniHelpers)

if(NOT ANDROID)
  find_package(JNI REQUIRED)
endif(NOT ANDROID)

include_directories(${JNI_INCLUDE_DIRS})

set(libJniHelpers_SOURCE_DIR src/main/cpp)
file(GLOB libJniHelpers_SOURCES ${libJniHelpers_SOURCE_DIR}/*.cpp)
file(GLOB libJniHelpers_HEADERS ${libJniHelpers_SOURCE_DIR}/*.h)

link_directories(${JNI_LIBRARIES})
add_library(${PROJECT_NAME} ${libJniHelpers_SOURCES} ${libJniHelpers_HEADERS})
