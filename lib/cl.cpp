#include "cl.h"
#include "utility.h"
#include "resource.h"

#include <CL/cl_gl.h>


#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include <iostream>

bool Amethyst_Verbose = true;

namespace amethyst {
namespace lib {
  
/// Constants
static const std::string SectionName("Amethyst OpenCL");
static const std::string ProcessVersion("0.1");
static const std::string ProcessString("OpenCL Processing Routines");

/// OpenCL Global State
std::vector<cl::Platform> cl_platforms;
std::vector<cl::Device> cl_devices;
cl::Context amethyst_cl_context;



void cl_init(void)
{

   try
    {
        cl::Platform::get(&cl_platforms);

        std::cout << SectionName << ": # OpenCL of platforms detected: " << cl_platforms.size() << std::endl;

        for(unsigned int i = 0; i < cl_platforms.size(); i++)
        {
            if (Amethyst_Verbose)
            {
                std::cout << SectionName << ": Platform[" << i << "] Vendor     : " << cl_platforms[0].getInfo<CL_PLATFORM_VENDOR>() << std::endl;
                std::cout << SectionName << ": Platform[" << i << "] Name       : " << cl_platforms[0].getInfo<CL_PLATFORM_NAME>() << std::endl;
                std::cout << SectionName << ": Platform[" << i << "] Version    : " << cl_platforms[0].getInfo<CL_PLATFORM_VERSION>() << std::endl;
                std::cout << SectionName << ": Platform[" << i << "] Extensions : " << cl_platforms[0].getInfo<CL_PLATFORM_EXTENSIONS>() << std::endl;
            }
        }

        /// Get OpenGL context sharing here!
        // Attempt to get OpenGL Context
        #ifdef __APPLE__
        cl_context_properties props[] =
        {
          CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
          0
        };
        try{
            std::cout << SectionName << ": Attempting to attach to CGL context." << std::endl;
            context = cl::Context(props);
            std::cout << SectionName << ": OpenGL integration successful." << std::endl;
        }
        catch (cl::Error er) {
            std::cout << SectionName << ": Warning! Failed to attach to CGL context (" << er.what() << ")" << std::endl;
            std::cout << SectionName << ": Creating context without OpenGL integration." << std::endl;
            cl_context_properties props_noGPU[] =
            {
                CL_CONTEXT_PLATFORM, (cl_context_properties)(cl_platforms[0])(),
                0
            };
            amethyst_cl_context = cl::Context(CL_DEVICE_TYPE_GPU, props_noGPU);
        }
        #elseif WIN32
        cl_context_properties props[] =
        {
            CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
            CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
            CL_CONTEXT_PLATFORM, (cl_context_properties)(cl_platforms[0])(),
            0
        };
        try{
          std::cout << SectionName << ": Attempting to attach to WGL context." << std::endl;
          amethyst_cl_context = cl::Context(CL_DEVICE_TYPE_GPU, props);
          std::cout << SectionName << ": OpenGL integration successful." << std::endl;
        }
        catch (cl::Error er) {
            std::cout << SectionName << ": Warning! Failed to attach to WGL context (" << er.what() << ")" << std::endl;
            std::cout << SectionName << ": Creating context without OpenGL integration." << std::endl;
            cl_context_properties props_noGPU[] =
            {
                CL_CONTEXT_PLATFORM, (cl_context_properties)(cl_platforms[0])(),
                0
            };
            amethyst_cl_context = cl::Context(CL_DEVICE_TYPE_GPU, props_noGPU);
        }
        #else
        cl_context_properties props[] =
        {
            CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties)(cl_context_properties)(cl_platforms[0])(),
            0
        };
         try{
            std::cout << SectionName << ": Attempting to attach to GLX context." << std::endl;
            amethyst_cl_context = cl::Context(CL_DEVICE_TYPE_GPU, props);
            std::cout << SectionName << ": OpenGL integration successful." << std::endl;
        }
        catch (cl::Error er) {
            std::cout << SectionName << ": Warning! Failed to attach to GLX context (" << er.what() << ")" << std::endl;
            std::cout << SectionName << ": Creating context without OpenGL integration." << std::endl;
            cl_context_properties props_noGPU[] =
            {
                CL_CONTEXT_PLATFORM, (cl_context_properties)(cl_platforms[0])(),
                0
            };
            amethyst_cl_context = cl::Context(CL_DEVICE_TYPE_GPU, props_noGPU);
        }
        #endif

        cl_devices = amethyst_cl_context.getInfo<CL_CONTEXT_DEVICES>();

        std::cout << SectionName << ": Devices detected in context[0]: " << cl_devices.size() << std::endl;

        for(unsigned int i = 0; i < cl_devices.size(); i++)
        {
            if (Amethyst_Verbose)
            {
                std::cout << SectionName << ": Device[" << i << "] Vendor        : " << cl_devices[0].getInfo<CL_DEVICE_VENDOR>() << std::endl;
                std::cout << SectionName << ": Device[" << i << "] Name          : " << cl_devices[0].getInfo<CL_DEVICE_NAME>() << std::endl;
                std::cout << SectionName << ": Device[" << i << "] Version       : " << cl_devices[0].getInfo<CL_DEVICE_VERSION>() << std::endl;
                std::cout << SectionName << ": Device[" << i << "] Extensions    : " << cl_devices[0].getInfo<CL_DEVICE_EXTENSIONS>() << std::endl;
                std::cout << SectionName << ": Device[" << i << "] Clock Freq    : " << cl_devices[0].getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << std::endl;
                std::cout << SectionName << ": Device[" << i << "] Compute Units : " << cl_devices[0].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
            }
        }

        std::cout << SectionName << ": " << cl_devices.size() << " device[s] initialized." << std::endl;

        //std::cout << SectionName << ": Loading Kernels..." << std::endl;
        //decode_cl_load_kernels();
        //std::cout << SectionName << ": Kernels loaded." << std::endl;

    }
    catch(...)
    {
        std::cout << SectionName << ": Initialization Failed!" << std::endl;
        //return 0;
    }

    //cl_context[0].get
    // Return the number of OpenCL devices discovered
    //return muir_cl_devices.size();
}

// Helper function to get error string
// *********************************************************************
const char* oclErrorString(cl_int error)
{
    static const char* errorString[] = {
      "CL_SUCCESS",
      "CL_DEVICE_NOT_FOUND",
      "CL_DEVICE_NOT_AVAILABLE",
      "CL_COMPILER_NOT_AVAILABLE",
      "CL_MEM_OBJECT_ALLOCATION_FAILURE",
      "CL_OUT_OF_RESOURCES",
      "CL_OUT_OF_HOST_MEMORY",
      "CL_PROFILING_INFO_NOT_AVAILABLE",
      "CL_MEM_COPY_OVERLAP",
      "CL_IMAGE_FORMAT_MISMATCH",
      "CL_IMAGE_FORMAT_NOT_SUPPORTED",
      "CL_BUILD_PROGRAM_FAILURE",
      "CL_MAP_FAILURE",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "",
      "CL_INVALID_VALUE",
      "CL_INVALID_DEVICE_TYPE",
      "CL_INVALID_PLATFORM",
      "CL_INVALID_DEVICE",
      "CL_INVALID_CONTEXT",
      "CL_INVALID_QUEUE_PROPERTIES",
      "CL_INVALID_COMMAND_QUEUE",
      "CL_INVALID_HOST_PTR",
      "CL_INVALID_MEM_OBJECT",
      "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
      "CL_INVALID_IMAGE_SIZE",
      "CL_INVALID_SAMPLER",
      "CL_INVALID_BINARY",
      "CL_INVALID_BUILD_OPTIONS",
      "CL_INVALID_PROGRAM",
      "CL_INVALID_PROGRAM_EXECUTABLE",
      "CL_INVALID_KERNEL_NAME",
      "CL_INVALID_KERNEL_DEFINITION",
      "CL_INVALID_KERNEL",
      "CL_INVALID_ARG_INDEX",
      "CL_INVALID_ARG_VALUE",
      "CL_INVALID_ARG_SIZE",
      "CL_INVALID_KERNEL_ARGS",
      "CL_INVALID_WORK_DIMENSION",
      "CL_INVALID_WORK_GROUP_SIZE",
      "CL_INVALID_WORK_ITEM_SIZE",
      "CL_INVALID_GLOBAL_OFFSET",
      "CL_INVALID_EVENT_WAIT_LIST",
      "CL_INVALID_EVENT",
      "CL_INVALID_OPERATION",
      "CL_INVALID_GL_OBJECT",
      "CL_INVALID_BUFFER_SIZE",
      "CL_INVALID_MIP_LEVEL",
      "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";

}


cl::Kernel cl_loadkernel ( const Resource& res, const std::string& mainfn )
{
  // Load cl Kernel source from file
  const std::string cl_source = res.to_str();

  std::cout << cl_source << std::endl;
  //cl::Program::Sources source = cl::Program::Sources(1, std::make_pair (cl_source.c_str(), cl_source.size()-1));
  cl::Program program = cl::Program(lib::amethyst_cl_context, cl_source);

  // Compile CL Kernel
  try {
    program.build(lib::cl_devices);
  }
  catch(...)
  {
    std::cout << "Build_log: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(lib::cl_devices[0]);
    throw;
  }

  /// FIXME XXX Log Warnings.
  std::cout << "Build_log: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(lib::cl_devices[0]);
  
  return cl::Kernel(program, mainfn.c_str());
}


} // Namespace lib
} // Namespace amethyst
