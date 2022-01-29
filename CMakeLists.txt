cmake_minimum_required(VERSION 3.21)
project(Chesscpp-main)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chesscpp-main main.cpp Board.cpp ui.cpp Pieces.cpp Headers/main.h Headers/Board.h Headers/ui.h Headers/Pieces.h)
