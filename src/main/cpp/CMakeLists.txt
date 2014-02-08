cmake_minimum_required(VERSION 2.8)
project(JniHelpersLib)

file(GLOB libJniHelpers_SOURCES *.cpp)
file(GLOB libJniHelpers_HEADERS *.h)

link_directories(${JNI_LIBRARIES})
add_library(JniHelpers SHARED ${libJniHelpers_SOURCES} ${libJniHelpers_HEADERS})
