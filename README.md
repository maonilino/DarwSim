DarwSim is a framework whose aim is to simulate a Darwinian evolution of a species in a specific environment. With a predetermined set of problems to be solved, genetic algorithms can be used. The end goal would be to be able to render this evolution process and make the renderer system flexible enough for the user to adapt it to their needs (e.g., the user of this frammework could use a completely different renderer system). This framework is designed following a plugin style architecture (i.e., the interface defines a set of functions that must be supported, and that the user can override).

# Supported features as of now
Please check the "related pages" category for the todo lists in the current framework state.

- [x] Linux Interface
- [ ] Windows Interface
- [x] OpenGL renderer
- [ ] Random environment generator
- [ ] Prey selection
- [ ] Random predator generation

# Known issues
Please check the "related pages" category for the list of currently known bugs.

# Building with cmake
As of now, the binaries can be found in the build folder of this project. It can be run with the following command, directly from the build directory:<br>  `./DarwSim` <br> 
Notice that it must be run form the build directory as there are several relative paths for loading the shaders and textures. <br> <br>
For compiling from source, cmake, opengl, glfw and glm libraries should be installed. Obviously, a recent gnu compiler is also required (any 6+ versions would do). These libraries are included in most linux distributions' package managers. For debian based distros, simply run: <br>
`sudo apt install libglfw3-dev libglm-dev` <br>
`unzip DarwSim.zip && cd DarwSim` <br>
`mkdir build && cd build` <br>
`cmake ..` <br>
`make -j4` or any higher number of cores that you wish to use <br>

# The MIT License (MIT)
Copyright © 2022 Gligorovski Nikola <br><br>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
