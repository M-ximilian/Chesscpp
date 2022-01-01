cmake_minimum_required(VERSION 3.21)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess Pieces.cpp Board.cpp Headers/Board.h Headers/Pieces.h main.cpp Headers/main.h ui.cpp Headers/ui.h)