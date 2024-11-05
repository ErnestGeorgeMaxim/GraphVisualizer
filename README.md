# Graph Visualizer

A C++ application for visualizing graphs, implemented using the SFML library. This project allows users to create nodes, connect them with edges, and view their relationships interactively in a graphical window.

## Features

- Create and display nodes on a 2D canvas.
- Connect nodes with directed or undirected edges.
- Drag and reposition nodes with collision avoidance.
- Load graph structure from an adjacency matrix text file.
- Smooth animations and customizable interface.

## Prerequisites

- **C++17 or higher**
- **SFML** (Simple and Fast Multimedia Library) - for handling graphics, windowing, and input.
- **CMake** - for project configuration and building.
- **Qt** (optional) - for using the Qt Design app or Visual Studio extension.

## Project Structure

```plaintext
GraphVisualizer
├── cmake-build-debug-visual-studio/  # Generated build files (Visual Studio specific)
├── Fonts/                            # Directory for font files
├── SFML/                             # SFML library files (ensure these are configured in your environment)
├── src/                              # Source code directory
│   ├── Edge.hpp                      # Edge class header file
│   ├── Graph.hpp                     # Graph class header file
│   ├── Node.hpp                      # Node class header file
│   └── main.cpp                      # Main entry point for the application
├── .gitignore                        # Git ignore file
├── adjacency_matrix.txt              # Input file for graph adjacency matrix
└── CMakeLists.txt                    # CMake configuration file
