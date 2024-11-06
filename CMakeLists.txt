cmake_minimum_required(VERSION 3.29)
project(LifeSim)

set(DEBUG 1)
set(CONSOLE 1)
set(CMAKE_CXX_STANDARD 23)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui)
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
set(OPENGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/opengl)
set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/glew-2.1.0-win32)
set(GLOBAL_INCLUDE_DIRS /lib /utils ${IMGUI_DIR} ${IMGUI_DIR}/backends)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4 /std:c++latest /Zi /Od /RTC1")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wformat -std=c++23")
endif ()

if (CONSOLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
endif ()

if (DEBUG)
    add_definitions(-DDEBUG)
endif ()

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
        source/Renderer.cpp
        source/Application.cpp
        source/ECSManager.cpp
        source/WorldManager.cpp
        source/MovementSystem.cpp
        source/RenderSystem.cpp
        source/CollisionSystem.cpp
)

# Add executable
if (WIN32)
    add_executable(LifeSim WIN32 ${SOURCES})
else ()
    add_executable(LifeSim ${SOURCES})
endif ()

# Platform-specific settings
if (APPLE)
    message("Building for Mac OS X")

    set(MAC_INCLUDE_DIRS
            /usr/local/include
            /opt/local/include
            /opt/homebrew/include
            ${GLFW_DIR}/glfw-3.4.bin.MACOS/include
            ${OPENGL_DIR}/Headers
            ${GLEW_DIR}/include
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
            glew
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
    )

    target_include_directories(LifeSim PRIVATE ${GLOBAL_INCLUDE_DIRS} ${MAC_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${MAC_LINK_DIRECTORIES})
    target_link_libraries(LifeSim PRIVATE ${MAC_LINK_LIBRARIES})
elseif (WIN32)
    message("Building for Windows")
    set(GLFW_INCLUDE_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/include)
    set(GLEW_LIBRARY_DIRS ${GLEW_DIR}/bin/Release/x64 ${GLEW_DIR}/lib/Release/x64)

    if (MSVC)
        set(GLFW_LIBRARY_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/lib-vc2022)
    else ()
        set(GLFW_LIBRARY_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)
    endif ()

    target_include_directories(LifeSim PRIVATE ${GLOBAL_INCLUDE_DIRS} ${OPENGL_DIR}/Headers ${GLEW_DIR}/include ${GLFW_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${OPENGL_DIR} ${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS})
    #    target_link_libraries(LifeSim PRIVATE gdi32 imm32 glfw3 glew32 glew32.lib, glu32.lib, and opengl32.lib)
    target_link_libraries(LifeSim PRIVATE gdi32 imm32 glew32.dll glfw3 glu32.dll opengl32.dll) # TODO: Why isn't glew linking correctly?

    # PkgConfig not working right now, and doesn't seem to be actually be necessary
    # find_package(PkgConfig REQUIRED)
    # pkg_check_modules(GLFW REQUIRED glfw3)
elseif (UNIX)
    message("Building for Linux")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(glew REQUIRED glew)
    pkg_check_modules(OpenGL REQUIRED gl)

    set(LINUX_INCLUDE_DIRS
            /usr/include
            /usr/local/include
            ${GLFW_DIR}/glfw-3.4.bin.MACOS/include
            ${OPENGL_DIR}/Headers
            ${GLEW_DIR}/include
    )
    set(LINUX_LINK_DIRECTORIES
            /usr/lib
            /usr/local/lib
    )
    set(LINUX_LINK_LIBRARIES
            glfw3
            glew
            gl
    )

    target_include_directories(LifeSim PRIVATE ${GLOBAL_INCLUDE_DIRS} ${LINUX_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${LINUX_LINK_DIRECTORIES})
    target_link_libraries(LifeSim PRIVATE ${LINUX_LINK_LIBRARIES})
endif ()
