# Introduction

![Screenshot with overlay modules installed](/docs/sceenshot.png?raw=true "Screenshot")

![Screenshot showing 3D Stereo](/docs/sceenshot-stereo.png?raw=true "Stereoscopic Demo")

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
