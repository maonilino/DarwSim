[![Build Status](https://app.travis-ci.com/maonilino/DarwSim.svg?branch=master)](https://app.travis-ci.com/maonilino/DarwSim)

DarwSim is a framework built using C++20 for my graduation project. The purpose of this project was to learn about Evolutionary computation techniques and review various topics learned during my undergraduate studies. This framework simulates a Darwinian evolution of a species in a specific environment. With a predetermined set of problems to be solved, genetic algorithms can be used. The end goal would be to be able to render this evolution process and make the framework flexible enough for the user to adapt it to their needs (e.g., the user of this frammework could use a completely different renderer system). This framework is designed following a plugin style architecture (i.e., the interface defines a set of functions that must be supported, and that the user can override).

# Supported features as of now
Please check the "related pages" category in the documentations for the todo lists in the current framework state.

- [x] Linux Interface
- [ ] Windows Interface
- [x] OpenGL renderer
- [x] Random environment generator
- [ ] Prey selection
- [ ] Random predator generation

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
