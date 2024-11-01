cmake_minimum_required(VERSION 3.29)
project(LifeSim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

#add_executable(
#        LifeSim main.cpp
#        extern/imgui/imgui.h
#        extern/imgui/imgui_impl_osx.mm
#        extern/imgui/imgui_impl_opengl2.h
#        #        extern/imgui/imgui.cpp
#        #        extern/imgui/imgui_impl_metal.mm
#        #        extern/imgui/imgui_impl_metal.h
#        #        extern/imgui/imgui_impl_osx.mm
#        #        WorldManager.cpp
#        #        WorldManager.h
#        #        Meep.cpp
#        #        Meep.h
#)

add_executable(
        LifeSim
        main.cpp
        extern/imgui/imgui_impl_osx.mm
        extern/imgui/imgui_impl_opengl2.h
        extern/imgui/imgui_impl_metal.mm
        extern/imgui/imgui.cpp
        extern/imgui/imgui_draw.cpp
        extern/imgui/imgui_demo.cpp
        extern/imgui/imgui_tables.cpp
        extern/imgui/imgui_widgets.cpp
)

target_link_libraries(LifeSim PRIVATE Cocoa Metal)