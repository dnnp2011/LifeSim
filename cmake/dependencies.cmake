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
    find_package(glew CONFIG REQUIRED)

    set(OpenGL_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/opengl")
    find_package(OpenGL REQUIRED)

    set(glm_DIR "${VCPKG_INSTALL_ROOT}/x64-windows/share/glm")
    find_package(glm CONFIG REQUIRED)
endfunction()
#endregion ---------------------------------------------------------------

#region Link vcpkg Dependencies -----------------------------------------------
function(link_dependencies)
    message(STATUS "Linking vcpkg dependencies...")

    target_link_libraries(LifeSim PUBLIC GLEW::GLEW)
    target_link_libraries(LifeSim PUBLIC OpenGL::GL)
    target_link_libraries(LifeSim PUBLIC glm::glm)
endfunction()
#endregion ---------------------------------------------------------------

#region Configure PkgConfig ----------------------------------------------
function(check_dependencies)
    message(STATUS "Checking dependencies with PkgConfig...")

    find_package(pkgconf CONFIG REQUIRED)
    pkg_check_modules(GLM REQUIRED glm)
    pkg_check_modules(GLEW REQUIRED glew32)
    pkg_check_modules(GL REQUIRED opengl32)
    #    pkg_check_modules(GLFW REQUIRED glfw3)
endfunction()
#endregion ---------------------------------------------------------------
