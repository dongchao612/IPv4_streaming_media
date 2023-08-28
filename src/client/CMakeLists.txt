cmake_minimum_required(VERSION 3.0.0)

project(CLIENT)

add_definitions(-DLOG_USE_COLOR)

set(HOME ~/IPv4_streaming_media)
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)

include_directories(${HOME}/src)

add_executable(client client.c ${HOME}/src/utils/log.c)
