cmake_minimum_required(VERSION 3.22.1)
project(LifeSim)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/macros.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

set(DEBUG 1)
set(CONSOLE 1)
set(USE_FETCH_CONTENT 0)
set(USE_VCPKG 1)
set(USE_PKG_CONFIG 0)
set(CMAKE_CXX_STANDARD 23)

#region Configure Global Settings ----------------------------------------
set_build_flags()
#endregion ---------------------------------------------------------------

#region Configure Dependencies -------------------------------------------
message(STATUS "Fetching dependencies...")

# Configure vcpkg
if(USE_VCPKG)
    set(VCPKG_ROOT ${CMAKE_SOURCE_DIR}/dependencies/vcpkg)
    set(VCPKG_INSTALL_ROOT ${CMAKE_SOURCE_DIR}/vcpkg_installed)
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    list(APPEND CMAKE_MODULE_PATH ${VCPKG_ROOT}/packages)
endif()

if(USE_FETCH_CONTENT)
    fetch_dependencies()
elseif(USE_VCPKG)
    find_dependencies()
endif()
#endregion ---------------------------------------------------------------

#region GLOBALS ------------------------------------------------------
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(USE_PKG_CONFIG)
    check_dependencies()
endif()

## ImGui
set(IMGUI_DIR ${DEPS_DIR}/imgui)

# Global Includes & Libraries
set(INCLUDE_DIRS
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        utils
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
        source/MovementSystem.cpp
        source/RenderSystem.cpp
        source/CollisionSystem.cpp
        utils/Random.h
        utils/Common.h
        utils/ServiceContainer.h
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

link_dependencies()
#endregion ---------------------------------------------------------------

#region Configure Platform-specific Settings -----------------------------
if(WIN32 OR WIN64)
    configure_windows_settings()
elseif(APPLE)
    configure_apple_settings()
elseif(UNIX)
    configure_unix_settings()
endif()
#endregion ---------------------------------------------------------------
