cmake_minimum_required(VERSION 3.23)
project(QuadTreeDevice VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory for header files
include_directories(include)

# Add all source files in the src/ directory
file(GLOB SOURCES "src/*.cpp")

# Create executable from sources
add_executable(main ${SOURCES})

# add json package
find_package(nlohmann_json REQUIRED)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

