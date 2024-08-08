cmake_minimum_required(VERSION 3.10)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)

# Find the spdlog package
find_package(spdlog CONFIG REQUIRED)

# Add your executable
add_executable(your_target main.cpp)

# Link spdlog with your executable
target_link_libraries(your_target PRIVATE spdlog::spdlog)
