cmake_minimum_required(VERSION 3.0.0)

project(SERVER)

add_definitions(-DLOG_USE_COLOR)

set(HOME ~/IPv4_streaming_media)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)
include_directories(${HOME}/src)

add_executable(server server.c ${HOME}/src/utils/log.c)
