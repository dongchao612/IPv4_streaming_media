cmake_minimum_required(VERSION 3.0.0)

project(CLIENT)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_definitions(-DLOG_USE_COLOR)

set(HOME /home/dongchao/桌面/git_project/IPv4_streaming_media)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)

include_directories(${HOME}/src)

add_executable(client client.c ${HOME}/src/utils/log.c)
