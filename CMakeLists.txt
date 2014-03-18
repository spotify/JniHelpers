cmake_minimum_required(VERSION 2.8)
project(JniHelpers)

if(ANDROID)
  #set(CMAKE_C_CFLAGS "${CMAKE_C_FLAGS} -D__GXX_EXPERIMENTAL_CXX0X__")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
else()
  find_package(JNI REQUIRED)
endif(ANDROID)
include_directories(${JNI_INCLUDE_DIRS})

add_subdirectory(src/main/cpp)
#add_subdirectory(src/test/cpp)
#add_subdirectory(src/test/data)

#add_dependencies(JniHelpersTest JniHelpersTestsGenerateData)
