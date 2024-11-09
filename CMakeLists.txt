cmake_minimum_required(VERSION 3.29)
project(LifeSim)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

set(DEBUG 1)
set(CONSOLE 1)
set(USE_FETCH_CONTENT 0)
set(USE_VCPKG 1)
set(USE_PKG_CONFIG 0)
set(USE_STATIC_LIBS 0)
set(USE_MANUAL_LINKING 0)
set(CMAKE_CXX_STANDARD 23)

#region Configure Global Settings ----------------------------------------
set_build_flags()
#set_global_variables()
#endregion ---------------------------------------------------------------


#region Configure Dependencies -------------------------------------------
message(STATUS "Fetching dependencies...")

# vcpkg
if(USE_VCPKG)
    set(VCPKG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vcpkg)
    set(VCPKG_INSTALL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed)
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_MODULE_PATH "${VCPKG_ROOT}/packages")
endif()

# PkgConfig
if(USE_PKG_CONFIG)
    set(PKG_CONFIG_EXECUTABLE ${DEPS_DIR}/vcpkg/packages/pkgconf_x64-windows/tools/pkgconf/pkgconf.exe)
endif()

if(USE_FETCH_CONTENT)
    fetch_dependencies()
elseif(USE_VCPKG)
    find_dependencies()
endif()

if(USE_PKG_CONFIG)
    check_dependencies()
endif()
#endregion ---------------------------------------------------------------

#region GLOBALS ------------------------------------------------------
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

## ImGui
set(IMGUI_DIR ${DEPS_DIR}/imgui)

# Global Includes & Libraries
set(INCLUDE_DIRS
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
set(LIBRARY_DIRS
        ${LIB_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
#endregion ---------------------------------------------------------------

#region Configure Executables --------------------------------------------
set(PROJECT_SOURCE
        LifeSim.cpp
        source/Renderer.cpp
        source/Application.cpp
        source/ECSManager.cpp
        source/WorldManager.cpp
        source/MovementSystem.cpp
        source/RenderSystem.cpp
        source/CollisionSystem.cpp
        utils/Random.h
        source/Common.h
)

set(IMGUI_SOURCE
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

if(WIN32 OR WIN64)
    add_executable(LifeSim WIN32 ${PROJECT_SOURCE} ${IMGUI_SOURCE})
else()
    add_executable(LifeSim ${PROJECT_SOURCE} ${IMGUI_SOURCE})
endif()

#add_executable(glewinfo glewinfo.c)
#target_link_libraries(glewinfo PRIVATE libglew_static)

link_dependencies()


#    if(WIN64)
#        set(GLFW_DIR ${GLFW_ROOT_DIR}/glfw-3.4.bin.WIN64)
#    elseif(APPLE)
#        set(GLFW_DIR ${GLFW_ROOT_DIR}/glfw-3.4.bin.MACOS)
#    elseif(UNIX)
#        message(FATAL_ERROR "Unix not supported for GLFW")
#    endif()

#    ## GLFW
#    set(GLFW_ROOT_DIR ${DEPENDENCIES_DIR}/glfw)
#    set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)

#    if(USE_STATIC_LIBS)
#        set(GLFW_LIBRARY_DIR ${GLFW_DIR}/lib-static-ucrt) # Static Library
#    else()
#        if(MSVC)
#            set(GLFW_LIBRARY_DIR ${GLFW_DIR}/lib-vc2022)
#        else()
#            set(GLFW_LIBRARY_DIR ${GLFW_DIR}/lib-mingw-w64)
#            #            set(GLFW_LIBRARY_DIR ${GLFW_DIR}/lib-static-ucrt)
#        endif()
#    endif()


#    set(GLFW_INCLUDE_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/include)

#    if(MSVC)
#        set(GLFW_LIBRARY_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/lib-vc2022)
#    else()
#        set(GLFW_LIBRARY_DIRS ${GLFW_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)
#    endif()

#    if(MSVC)
#        set_target_properties(LifeSim PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#    endif()

#if(USE_STATIC_LIBS)
#    add_library(glfw3 STATIC IMPORTED)
#    set_target_properties(glfw3 PROPERTIES
#            IMPORTED_LOCATION "${GLFW_LIBRARY_DIR}/glfw3dll.lib"
#            IMPORTED_IMPLIB "${GLFW_LIBRARY_DIR}/glfw3dll.lib"
#            INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
#    )
#else()
#    add_library(glfw3 SHARED IMPORTED)
#    if(MSVC)
#        set_target_properties(glfw3 PROPERTIES
#                IMPORTED_LOCATION "${GLFW_LIBRARY_DIR}/glfw3.dll"
#                IMPORTED_IMPLIB "${GLFW_LIBRARY_DIR}/glfw3.lib"
#                INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
#        )
#    else()
#        set_target_properties(glfw3 PROPERTIES
#                IMPORTED_LOCATION "${GLFW_LIBRARY_DIR}/glfw3.dll"
#                IMPORTED_IMPLIB "${GLFW_LIBRARY_DIR}/libglfw3dll.a"
#                INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
#        )
#    endif()
#endif()
#target_link_libraries(LifeSim PRIVATE glfw3)

#endregion ---------------------------------------------------------------

#region Configure Platform-specific Settings -----------------------------
if(WIN64)
    configure_windows_settings()
elseif(APPLE)
    configure_apple_settings()
elseif(UNIX)
    configure_unix_settings()
endif()
#endregion ---------------------------------------------------------------
