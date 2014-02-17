cmake_minimum_required(VERSION 2.8)
project(JniHelpersLib)

file(GLOB libJniHelpers_SOURCES *.cpp)
file(GLOB libJniHelpers_HEADERS *.h)

link_directories(${JNI_LIBRARIES})
add_library(JniHelpers ${libJniHelpers_SOURCES} ${libJniHelpers_HEADERS})
set_target_properties(JniHelpers PROPERTIES CMAKE_SHARED_LINKER_FLAGS "-fPIC")
if(${UNIX})
  set(CMAKE_CXX_FLAGS "-fPIC")
endif(${UNIX})
