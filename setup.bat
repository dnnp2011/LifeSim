@echo off

:: Install any git submodules
git submodule update --init --recursive

:: Add vcpkg to PATH
set VCPKG_ROOT=%CD%\dependencies\vcpkg
setx VCPKG_ROOT %CD%\dependencies\vcpkg
set PATH=%PATH%;%VCPKG_ROOT%
setx PATH %PATH%:%VCPKG_ROOT%

:: Bootstrap vcpkg
call %VCPKG_ROOT%\bootstrap-vcpkg.bat

echo "VCPKG_ROOT is set to %VCPKG_ROOT%"
echo "PATH was updated to include vcpkg"
echo "\n-------------------------------------"

:: Verify VCPKR_ROOT exists and is included in PATH
vcpkg version

echo "\n-------------------------------------"
:: Create vcpkg.json file if it does not exist
if not exist %CD%\vcpkg.json (
    echo "No vcpkg.json file found. Creating one now..."

    vcpkg new --application

    echo "Adding dependencies..."
    vcpkg add port glew
    vcpkg add port glfw3
    vcpkg add port opengl
    vcpkg add port glm
    vcpkg add port pkgconf

    echo "Installing dependencies..."
    vcpkg install --triplet x64-windows
) else (
    echo "The file does not exist."
)

echo "Setup Complete!"
