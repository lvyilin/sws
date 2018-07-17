cmake_minimum_required(VERSION 3.10)
project(sws C)

set(CMAKE_C_STANDARD 99)

add_executable(sws main.c network.h network.c logger.h parser.h response.h logger.c fileio.h fileio.c version.h response.c utility.h parser.c utility.c)