# Use an official GCC image as the base
FROM ubuntu:latest

# Install CMake and other necessary packages
RUN apt-get update && \
    apt-get install -y build-essential cmake wget && \
    rm -rf /var/lib/apt/lists/*

# Install nlohmann/json.hpp
RUN mkdir -p /usr/local/include/nlohmann && \
    wget https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp -O /usr/local/include/nlohmann/json.hpp

# Set the working directory
WORKDIR /QuadTreeDevice

# Copy the CMakeLists.txt and your source code into the container
COPY CMakeLists.txt /QuadTreeDevice/
COPY src/ /QuadTreeDevice/src/
COPY include/ /QuadTreeDevice/include/
COPY data/ /QuadTreeDevice/data/

# Create a build directory and run cmake to generate build files
RUN mkdir build && \
    cd build && \
    cmake ..

# Compile the project
RUN cmake --build /QuadTreeDevice/build

# Specify the command to run on container start
CMD ["/QuadTreeDevice/build/main", "--author"]
