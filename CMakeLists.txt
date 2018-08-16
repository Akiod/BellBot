cmake_minimum_required(VERSION 3.7.2)
project(bellbot)

file(GLOB SOURCES "src/*.cpp")

add_executable(bellbot ${SOURCES})
