# Introduction

This is my pet project to explore iterative integration and visualization techniques. My dream was to turn this into a game or multiplayer simulator with realistic physics, but I've since decided to put this out there as an example of a working gravity simulator with high-precision with other goodies.

This project uses OpenGL 3.3 Core and is (was, currently untested) multi-platform. While I was a student at the University of Alaska Fairbanks (UAF) studying physics, I made an extensive overhaul to the engine to use Runge-Kutta 4th order integration as well as make use of GPGPU hardware. I used this as a tool for my undergraduate research project to explore the Interplanetary Transport Network (ITN) which is a set of pathways throughout our solar system that allow travel using little to no energy, that is, if time is not an issue.

It supports module loading, bmp texture loading, custom shaders, stereoscopic display, loading and saving state via XML and HDF5, offloading O(n^2) gravity calculation to GPU via OpenCL, and multiplayer (currently broken).  There are a few modules that can be loaded to show more science related visuals, such as gravimetric potential (wells) over the whole system.

![Screenshot with overlay modules installed](/docs/screenshot.png?raw=true "Screenshot")

A screenshot showing U/D stereoscopic visual suitable for some 3D televisions.

![Screenshot showing 3D Stereo](/docs/screenshot-stereo.png?raw=true "Stereoscopic Demo")

Demo of Gravimetric Potential Plane visualization module.

![Gravimetric Potential](/docs/potplane_3.png?raw=true "Gravimetric Potential Plane")

# Build Requirements

- [ ] OpenGL
- [ ] OpenCL
- [ ] OpenAL
- [ ] LibSDL2
- [ ] libXML
- [ ] GLEW
- [ ] GLUT
- [ ] HDF5

If GUI components (OpenGL, X11, etc) are missing during build, only the library, server, and commandline utilities will be built.

## on Fedora
`dnf install libxml2-devel SDL2-devel SDL2-static glew-devel hdf5-devel ocl-icd-devel openal-devel glut-devel`

# Building
`mkdir build`

`cmake .. -D OpenGL_GL_PREFERENCE=GLVND`

# Prepping for first run
## Populate resouce directory in your home directory. ( Ex. ~/.amethyst/)
## Move modules into home directory
