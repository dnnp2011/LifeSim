cmake_minimum_required(VERSION 3.10)
project(LifeSim)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wformat -std=c++11")

# Define source files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)
set(SOURCES
        main.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

# Add executable
add_executable(LifeSim ${SOURCES})

# Include directories
target_include_directories(LifeSim PRIVATE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Platform-specific settings
if(APPLE)
    message("Building for Mac OS X")
    target_link_libraries(LifeSim PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-L/usr/local/Cellar/glfw/3.4/lib"
            "-lglfw"
    )
    target_include_directories(LifeSim PRIVATE
            /usr/local/include
            /opt/local/include
            /opt/homebrew/include
            /usr/local/Cellar/glfw/3.4/include
    )
    link_directories(
            /usr/local/lib
            /opt/local/lib
            /opt/homebrew/lib
            /usr/local/Cellar/glfw/3.4/lib
            /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks
    )
elseif(UNIX)
    message("Building for Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_link_libraries(LifeSim PRIVATE ${GLFW_LIBRARIES} GL)
    target_include_directories(LifeSim PRIVATE ${GLFW_INCLUDE_DIRS})
elseif(WIN32)
    message("Building for Windows")
    target_link_libraries(LifeSim PRIVATE glfw3 gdi32 opengl32 imm32)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_include_directories(LifeSim PRIVATE ${GLFW_INCLUDE_DIRS})
endif()