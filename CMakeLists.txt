cmake_minimum_required(VERSION 3.16)
project(z80)

#include($ENV{HOME}/.cmake/CommonIncludes.cmake)
#include(inc/version.hpp)

#configure_file(src/Version.hpp.in ${CMAKE_BINARY_DIR}/Version.hpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

# display short filename in logs
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/src/,,$(abspath $<))\"'")

# Svg needs: apt-get install libqt5svg5-dev
#find_package(Qt5 COMPONENTS Widgets Svg REQUIRED)
#find_package(titanium-sdk COMPONENTS userinterface core REQUIRED)
#find_package(rbc-data-model)
#find_package(Poco COMPONENTS Util Foundation)

set(SRC_FILES src/z80.cpp
    src/Register_8bits.cpp
    src/Register_F.cpp
    src/Register_16bits.cpp
    src/Memory.cpp
    src/Label.cpp
    src/LabelDataset.cpp
    src/Z80Machine.cpp)       

#add_definitions(-DQT_DEPRECATED_WARNINGS)

add_executable(${PROJECT_NAME} ${SRC_FILES})

string(TIMESTAMP VERSION_RELEASE "-%m%dT%H%M")

install(TARGETS ${PROJECT_NAME} RUNTIME)

