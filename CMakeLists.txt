cmake_minimum_required(VERSION 3.29)
project(GraphVisualizerr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "D:/clion_projects/GraphVisualizerr/SFML/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/Node.hpp
        src/Node.hpp
        src/Edge.hpp
        src/Graph.hpp)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
