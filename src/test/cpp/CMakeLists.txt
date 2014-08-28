# Copyright (c) 2014 Spotify AB

cmake_minimum_required(VERSION 2.8)
project(JniHelpersTest)

file(GLOB libJniHelpersTest_SOURCES *.cpp)
file(GLOB libJniHelpersTest_HEADERS *.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../main/cpp)
link_directories(${JNI_LIBRARIES})
add_library(${PROJECT_NAME} SHARED ${libJniHelpersTest_SOURCES} ${libJniHelpersTest_HEADERS})
target_link_libraries(${PROJECT_NAME} JniHelpers)

if(${UNIX})
  set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(${WINDOWS})
  set(CMAKE_CXX_FLAGS "/MP /EHsc")
endif(${UNIX})
