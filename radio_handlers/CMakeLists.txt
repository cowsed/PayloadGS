cmake_minimum_required(VERSION 3.15)

project(RadioHandler)


add_executable(radio_handler main.cpp)
target_include_directories(radio_handler PRIVATE "sx1262/src")