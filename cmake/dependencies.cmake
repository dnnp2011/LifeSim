#region FetchContent Dependencies ----------------------------------------
function(fetch_dependencies)
    message(STATUS "Downloading dependencies with FetchContent...")

    include(FetchContent)

    #region GLM --------------------------------------------------------------
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
    )
    FetchContent_MakeAvailable(glm)
    #endregion ---------------------------------------------------------------

    #region GLEW -------------------------------------------------------------
    FetchContent_Declare(
            glew
            GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
            GIT_TAG origin/master
    )
    FetchContent_GetProperties(glew)

    if(NOT glew_POPULATED)
        FetchContent_Populate(glew)

        add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
    #endregion ---------------------------------------------------------------
endfunction()
#endregion ---------------------------------------------------------------

#region Find vcpkg Dependencies -----------------------------------------------
function(find_dependencies)
    message(STATUS "Finding vcpkg dependencies...")

    set(glew_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/glew")
    set(glew_FOUND TRUE)
    find_package(glew CONFIG REQUIRED)

    set(OpenGL_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/opengl")
    find_package(OpenGL REQUIRED)

    set(glm_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/glm")
    set(glm_FOUND TRUE)
    find_package(glm CONFIG REQUIRED)

    set(glfw3_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/glfw3")
    find_package(glfw3 CONFIG REQUIRED)
endfunction()
#endregion ---------------------------------------------------------------

#region Link vcpkg Dependencies -----------------------------------------------
function(link_dependencies)
    message(STATUS "Linking vcpkg dependencies...")

    set(GLEW_INCLUDE_DIR ${VCPKG_INSTALL_ROOT}/x64-windows/include)
    set(GLEW_LIBRARY_DIR ${VCPKG_INSTALL_ROOT}/x64-windows/lib)
    set(GLEW_DEBUG_LIBRARY_DIR ${VCPKG_INSTALL_ROOT}/x64-windows/debug/lib)
    set(GLEW_BIN_DIR ${VCPKG_INSTALL_ROOT}/x64-windows/bin)
    set(GLEW_DEBUG_BIN_DIR ${VCPKG_INSTALL_ROOT}/x64-windows/debug/bin)

    # Fix for missing glew32.lib reference
    set_target_properties(GLEW::GLEW PROPERTIES
            IMPORTED_IMPLIB "${GLEW_LIBRARY_DIR}/glew32.lib"
            IMPORTED_IMPLIB_DEBUG "${GLEW_DEBUG_LIBRARY_DIR}/glew32d.lib"
            IMPORTED_LINK_INTERFACE_LIBRARIES "opengl32;glu32"
            IMPORTED_LOCATION "${GLEW_BIN_DIR}/glew32.dll"
            IMPORTED_LOCATION_DEBUG "${GLEW_DEBUG_BIN_DIR}/glew32d.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
    )

    target_link_libraries(LifeSim PUBLIC GLEW::GLEW)
    target_link_libraries(LifeSim PUBLIC OpenGL::GL)
    target_link_libraries(LifeSim PUBLIC glm::glm)
    target_link_libraries(LifeSim PUBLIC glfw)
endfunction()
#endregion ---------------------------------------------------------------

#region Configure PkgConfig ----------------------------------------------
function(check_dependencies)
    message(STATUS "Checking dependencies with PkgConfig...")
    include(FindPkgConfig)
    set(PKG_CONFIG_EXECUTABLE ${DEPS_DIR}/vcpkg/packages/pkgconf_x64-windows/tools/pkgconf/pkgconf.exe)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(glew REQUIRED glew)
    pkg_check_modules(OpenGL REQUIRED OpenGL)
    pkg_check_modules(glm REQUIRED glm)
    pkg_check_modules(glfw3 REQUIRED glfw3)
endfunction()
#endregion ---------------------------------------------------------------
