cmake_minimum_required(VERSION 3.29)
project(LifeSim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

# Add the GLFW include directory
include_directories("/usr/local/Cellar/glfw/3.4/include")
# Add the ImGui include directory
include_directories(${CMAKE_SOURCE_DIR}/extern/imgui)


#add_executable(
#        LifeSim main.cpp
#extern/imgui/imgui_impl_opengl2.h
#extern/imgui/imgui_impl_glfw.h
#extern/imgui/imgui.cpp
#extern/imgui/imgui_draw.cpp
#extern/imgui/imgui_demo.cpp
#extern/imgui/imgui_tables.cpp
#extern/imgui/imgui_widgets.cpp
#        #        WorldManager.cpp
#        #        WorldManager.h
#        #        Meep.cpp
#        #        Meep.h
#)

add_executable(
        LifeSim
        main.cpp
)

target_link_libraries(LifeSim PRIVATE Cocoa Metal)

# Link the GLFW and other necessary libraries
target_link_libraries(LifeSim PRIVATE /usr/local/Cellar/glfw/3.4/lib/libglfw.dylib "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo" "-framework Metal" "-framework QuartzCore" )
# Add the macOS frameworks directory to the linker search paths
link_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks)
# Ensure the linker can find the Cocoa framework
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks")