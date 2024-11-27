#region Set Build Flags --------------------------------------------------
macro(set_build_flags)
    message(STATUS "Setting Build Flags")

    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /std:c++23")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++23")
    endif()

    if(CONSOLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
    endif()

    if(DEBUG)
        add_definitions(-DDEBUG)

        if(MSVC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /Zi") #/RTC1
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wformat -Wpedantic")
        endif()
    else()
        if(MSVC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
        endif()
    endif()

    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endmacro()
#endregion ---------------------------------------------------------------

#region Build for Windows ------------------------------------------------
macro(configure_windows_settings)
    message(STATUS "Building for Windows...")

    target_include_directories(LifeSim PRIVATE ${INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${LIBRARY_DIRS})

    # Other Platform Libraries
    target_link_libraries(LifeSim PRIVATE gdi32 imm32)
endmacro()
#endregion ---------------------------------------------------------------

#region Build for Apple/Mac ----------------------------------------------
macro(configure_apple_settings)
    message("Building for Mac OS X")

    set(MAC_INCLUDE_DIRS
            /usr/local/include
            /opt/local/include
            /opt/homebrew/include
    )
    set(MAC_LINK_DIRECTORIES
            /usr/local/lib
            /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks
    )
    set(MAC_LINK_LIBRARIES
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
    )

    target_include_directories(LifeSim PRIVATE ${INCLUDE_DIRS} ${MAC_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${MAC_LINK_DIRECTORIES})
    target_link_libraries(LifeSim PRIVATE ${LIBRARY_DIRS} ${MAC_LINK_LIBRARIES})
endmacro()
#endregion ---------------------------------------------------------------

#region Build for Linux --------------------------------------------------
macro(configure_linux_settings)
    message("Building for Linux")

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
            glm
    )

    target_include_directories(LifeSim PRIVATE ${INCLUDE_DIRS} ${LINUX_INCLUDE_DIRS})
    target_link_directories(LifeSim PRIVATE ${LINUX_LINK_DIRECTORIES})
    target_link_libraries(LifeSim PRIVATE ${LINUX_LINK_LIBRARIES})
endmacro()
#endregion ---------------------------------------------------------------
