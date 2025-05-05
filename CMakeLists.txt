cmake_minimum_required(VERSION 3.31)
project(MiniMaze4)

set(CMAKE_CXX_STANDARD 20)

add_executable(MiniMaze4 main.cpp
        object.cpp
        object.h
        Room.cpp
        Room.h
        Profile.cpp
        Profile.h
        Player.cpp
        Player.h
        Game.cpp
        Game.h
        Enemy.cpp
        Enemy.h)
