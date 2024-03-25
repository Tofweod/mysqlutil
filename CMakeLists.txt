cmake_minimum_required(VERSION 3.20)

project(MysqlUtil)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.c)
include_directories(${PROJECT_SOURCE_DIR})

message("file is " ${SRC})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/so)
add_library(mysqlutil SHARED ${SRC})
