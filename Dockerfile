# Use a lightweight Alpine base image
FROM alpine:latest

# Set the working directory to /LifeSim
WORKDIR /LifeSim

# Install dependencies
RUN apk update && apk add --no-cache \
    gcc \
    g++ \
    make \
    cmake \
    ninja-build \
    perf \
    openssh \
    openssl \
    sudo

# Install common dependencies
RUN apk update && apk add --no-cache \
    bash \
    gcc \
    g++ \
    make \
    cmake \
    ninja-build \
    gdb \
    openssh \
    openssl \
    build-base \
    cmake \
    ninja \
    zip \
    unzip \
    curl \
    git \
    glib-dev \
    sudo

# Install additional packages
RUN apk add --no-cache \
     perf

# Set environment variables \
ENV VCPKG_ROOT="/LifeSim/dependencies/vcpkg" \
    VCPKG_DEFAULT_TRIPLET="x64-linux" \
    CC="gcc" \
    CXX="g++" \
    CMAKE_BUILD_TYPE="Debug" \
    VCPKG_DISABLE_METRICS=1 \
    VCPKG_CMAKE_SYSTEM_NAME="Linux"
ENV PATH="$VCPKG_ROOT:/LifeSim/bin:$PATH" \
    CMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

# Set up SSH & set root password
RUN mkdir /var/run/sshd && \
    echo "root:root" | chpasswd && \
    sed -i 's/#PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    sed -i 's/#PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
    sed -i 's/.*UsePAM.*/UsePAM no/' /etc/ssh/sshd_config && \
    ssh-keygen -A

# Copy the current directory contents into the container at /LifeSim
COPY . /LifeSim

# Expose SSH port
EXPOSE 22

# Start SSH service
CMD ["/usr/sbin/sshd", "-D"]
