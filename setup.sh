#!/usr/bin/env bash

# Install any git submodules
git submodule update --init --recursive

# Add vcpkg to PATH
export VCPKG_ROOT=$(pwd)/dependencies/vcpkg
export PATH=$VCPKG_ROOT:$PATH

# Bootstrap vcpkg
$VCPKG_ROOT/bootstrap-vcpkg.sh

echo "VCPKG_ROOT is set to $VCPKG_ROOT"
echo "PATH was updated to include vcpkg"
echo "\n-------------------------------------"

# Verify VCPKG_ROOT exists and is included in PATH
vcpkg version

echo "\n-------------------------------------"
# Determine OS and set triplet
if [[ "$OSTYPE" == "darwin"* ]]; then
    TRIPLET="x64-osx"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    TRIPLET="x64-linux"
elif [[ "$OSTYPE" == "msys"* ]]; then
    TRIPLET="x64-windows"
else
    echo "Unsupported OS type: $OSTYPE"
    exit 1
fi

# Create vcpkg.json file if it does not exist
if [[ ! -f "vcpkg.json" ]]; then
    echo "No vcpkg.json file found. Creating one now..."

    vcpkg new --application

    echo "Adding dependencies..."
    vcpkg add port glew
    vcpkg add port glfw3
    vcpkg add port opengl
    vcpkg add port glm
    vcpkg add port pkgconf

    echo "Installing dependencies..."
    vcpkg install --triplet $TRIPLET
else
    echo "vcpkg.json file already exists."
fi

echo "Setup Complete!"
