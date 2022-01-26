cmake_minimum_required(VERSION 3.10)

project(YAWC)

add_executable(yawc main.c board.c words.c game.c)
target_link_libraries(yawc m)