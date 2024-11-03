cmake_minimum_required(VERSION 3.29)
project(LifeSim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wformat -std=c++23")
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
set(OPENGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/opengl)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS") // Causes "No such file or directory" error


# Define source files
set(SOURCES
        LifeSim.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

# Add executable
if (WIN32)
    add_executable(LifeSim WIN32 ${SOURCES})
else ()
    add_executable(LifeSim ${SOURCES})
endif ()

# Include directories
target_include_directories(LifeSim PRIVATE /lib ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Platform-specific settings
if (APPLE)
    message("Building for Mac OS X")

    set(MAC_INCLUDE_DIRS
            /usr/local/include
            /opt/local/include
            /opt/homebrew/include
            ${GLFW_DIR}/glfw-3.4.bin.MACOS/include
            ${OPENGL_DIR}
    )
    set(MAC_LINK_DIRECTORIES
            /usr/local/lib
            /opt/local/lib
            /opt/homebrew/lib
            /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks
            ${GLFW_DIR}/glfw-3.4.bin.MACOS/lib-x86_64
            ${OPENGL_DIR}
    )
    set(MAC_LINK_LIBRARIES
            glfw3
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            #            "-framework CoreVideo"
    )

    target_include_directories(LifeSim PRIVATE ${MAC_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${MAC_LINK_DIRECTORIES})
    target_link_libraries(LifeSim PRIVATE ${MAC_LINK_LIBRARIES})
elseif (WIN32)
    message("Building for Windows")

    set(GLFW_INCLUDE_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/include)
    set(GLFW_LIBRARY_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)

    target_include_directories(LifeSim PRIVATE ${GLFW_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${GLFW_LIBRARY_DIRS} ${OPENGL_DIR})
    target_link_libraries(LifeSim PRIVATE glfw3 gdi32 opengl32 imm32)

    # PkgConfig not working right now, and doesn't seem to be actually be necessary
    # find_package(PkgConfig REQUIRED)
    # pkg_check_modules(GLFW REQUIRED glfw3)
elseif (UNIX)
    message("Building for Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    target_link_libraries(LifeSim PRIVATE ${GLFW_LIBRARIES} GL)
    target_include_directories(LifeSim PRIVATE ${GLFW_INCLUDE_DIRS})
endif ()
