# Introduction

This is my pet project to explore iterative integration and visualization techniques. My dream was to turn this into a game or multiplayer simulator with realistic physics, but I've since decided to put this out there as an example of a working gravity simulator with high-precision with other goodies.

This project uses OpenGL 3.3 Core and is (was, currently untested) multi-platform. While I was a student at the University of Alaska Fairbanks (UAF) studying physics, I made an extensive overhaul to the engine to use Runge-Kutta 4th order integration as well as make use of GPGPU hardware. I used this as a tool for my undergraduate research project to explore the Interplanetary Transport Network (ITN) which is a set of pathways throughout our solar system that allow travel using little to no energy, that is, if time is not an issue.

It supports module loading, bmp texture loading, custom shaders, stereoscopic display, loading and saving state via YAML and HDF5, offloading O(n^2) gravity calculation to GPU via OpenCL, and multiplayer (currently broken).  There are a few modules that can be loaded to show more science-related visuals, such as gravimetric potential (wells) over the whole system.

![Screenshot with overlay modules installed](/docs/screenshot.png?raw=true "Screenshot")

A screenshot showing U/D stereoscopic visual suitable for some 3D televisions.

![Screenshot showing 3D Stereo](/docs/screenshot-stereo.png?raw=true "Stereoscopic Demo")

Demo of Gravimetric Potential Plane visualization module.

![Gravimetric Potential](/docs/potplane_3.png?raw=true "Gravimetric Potential Plane")

# Build Requirements

- [ ] Boost
- [ ] Simple DirectMedia Layer v2 (libSDL2)
- [ ] OpenGL
- [ ] OpenCL
- [ ] OpenAL
- [ ] yaml-cpp (as a git subproject)
- [ ] GLEW
- [ ] GLUT
- [ ] HDF5

If GUI components (OpenGL, X11, etc) are missing during build, only the library, server, and commandline utilities will be built.

## on Fedora
`dnf install boost-devel boost-static freetype-devel libxml2-devel SDL2-devel SDL2-static glew-devel hdf5-devel ocl-icd-devel openal-devel glut-devel vulkan-loader-devel vulkan-headers vulkan-validation-layers-devel`

# Building
`mkdir build && cd build`

`cmake ..`

# Prepping for first run
## Populate resource directory in your home directory. ( Ex. ~/.amethyst/)
TODO - the resources that are used in the screenshots are not distributable.  I'm working a solution for that.
## Move modules into home directory
