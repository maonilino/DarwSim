[![Build Status](https://app.travis-ci.com/maonilino/DarwSim.svg?branch=master)](https://app.travis-ci.com/maonilino/DarwSim) [![CMake](https://github.com/maonilino/DarwSim/actions/workflows/cmake.yml/badge.svg)](https://github.com/maonilino/DarwSim/actions/workflows/cmake.yml)

[![CMake](https://github.com/maonilino/DarwSim/actions/workflows/cmake.yml/badge.svg)](https://github.com/maonilino/DarwSim/actions/workflows/cmake.yml)

DarwSim is a framework built using C++20 as part of an academic graduation project. The purpose of this undertaking is to learn and experiment with Evolutionary computation techniques and data-structures algorithms. Through this experiment I expect to review various topics learned during my undergraduate studies. This framework aim’s is to provide a framework with an interface for simulating a Darwinian evolution of a species in its environment. If the species is given a predetermined set of problems to solve, a set of genetic algorithms can be used. Because a proper NP-Hard or NP-Complete problem formulation can be a daunting task to accomplish at an undergraduate level, for this paper, the problem is simply formulated as a simple NP problem such as the shortest path problem, where the goal is to use genetic algorithms to solve it. This framework implements Gen, Cheng, and Wang, ‘Genetic Algorithms for Solving Shortest Path Problems’. The end goal for this framework would be to render this process. In order to make this framework flexible enough for other users to adapt it to their needs (e.g., the user of this framework could use a completely different renderer system). This framework is designed following a plugin style architecture (i.e., the interface defines a set of functions that must be supported, and that the user can override).

# Supported features as of now
Please check the "related pages" category in the documentations for the todo lists in the current framework state.

- [x] Linux Interface
- [ ] Windows Interface
- [x] OpenGL renderer
- [x] Random environment generator
- [x] Gen, Cheng, and Wang, ‘Genetic Algorithms for Solving Shortest Path Problems’ Implementations

# Known issues
Please check the "related pages" category for the list of currently known bugs.

# Building with cmake
As of now, the binaries can be found in the build folder of this project. It can be run with the following command, directly from the build directory:<br>  `./DarwSim` <br> 
Notice that it must be run form the build directory as there are several relative paths for loading the shaders and textures. <br> <br>
For compiling from source, cmake, opengl, glfw and glm libraries should be installed. Obviously, a recent gnu compiler is also required (as this project uses C++20, any 8.3+ versions should work). These libraries are included in most linux distributions' package managers. For debian based distros, simply run: <br>
`sudo apt install libglfw3-dev libglm-dev libglew-dev libglfw3-dev libglm-dev libstb-dev` <br>
`unzip DarwSim.zip && cd DarwSim` <br>
`mkdir build && cd build` <br>
`cmake ..` <br>
`make -j4` or any higher number of cores that you wish to use <br>
