cmake_minimum_required(VERSION 3.29)
project(LifeSim)

set(CMAKE_CXX_STANDARD 23)

#add_subdirectory(extern/imgui)
add_executable(
        LifeSim main.cpp
        extern/imgui/imgui_impl_metal.mm
        extern/imgui/imgui_impl_metal.h
        extern/imgui/imgui_impl_osx.mm
        extern/imgui/imgui_impl_osx.h
        WorldManager.cpp
        WorldManager.h
        Meep.cpp
        Meep.h
)