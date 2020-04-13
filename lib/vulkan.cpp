#include "vulkan.hpp"

#include "lib/console_io.h"

#include <inttypes.h>
#include <functional>
#include <sstream>
#include <iostream>

// Macro to check and display Vulkan return results
#if defined(__ANDROID__)
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        LOGE("Fatal : VkResult is \" %s \" in %s at line %d", amethyst::lib::errorString(res).c_str(), __FILE__, __LINE__); \
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#else
#define VK_CHECK_RESULT(f)																				\
{																										\
    VkResult res = (f);																					\
    if (res != VK_SUCCESS)																				\
    {																									\
        std::cout << "Fatal : VkResult is \"" << amethyst::lib::errorString(res) << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
        assert(res == VK_SUCCESS);																		\
    }																									\
}
#endif


namespace amethyst::lib {

std::string errorString(VkResult errorCode)
{
    switch (errorCode)
    {
        #define STR(r) case VK_ ##r: return #r
        STR(NOT_READY);
        STR(TIMEOUT);
        STR(EVENT_SET);
        STR(EVENT_RESET);
        STR(INCOMPLETE);
        STR(ERROR_OUT_OF_HOST_MEMORY);
        STR(ERROR_OUT_OF_DEVICE_MEMORY);
        STR(ERROR_INITIALIZATION_FAILED);
        STR(ERROR_DEVICE_LOST);
        STR(ERROR_MEMORY_MAP_FAILED);
        STR(ERROR_LAYER_NOT_PRESENT);
        STR(ERROR_EXTENSION_NOT_PRESENT);
        STR(ERROR_FEATURE_NOT_PRESENT);
        STR(ERROR_INCOMPATIBLE_DRIVER);
        STR(ERROR_TOO_MANY_OBJECTS);
        STR(ERROR_FORMAT_NOT_SUPPORTED);
        STR(ERROR_SURFACE_LOST_KHR);
        STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        STR(SUBOPTIMAL_KHR);
        STR(ERROR_OUT_OF_DATE_KHR);
        STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        STR(ERROR_VALIDATION_FAILED_EXT);
        STR(ERROR_INVALID_SHADER_NV);
        #undef STR
        default:
            return "UNKNOWN_ERROR";
    }
}

std::string physicalDeviceTypeString(VkPhysicalDeviceType type)
{
    switch (type)
    {
        #define STR(r) case VK_PHYSICAL_DEVICE_TYPE_ ##r: return #r
        STR(OTHER);
        STR(INTEGRATED_GPU);
        STR(DISCRETE_GPU);
        STR(VIRTUAL_GPU);
        #undef STR
        default: return "UNKNOWN_DEVICE_TYPE";
    }
}



ConsoleIO* VulkanCompute::io_s = nullptr;

VulkanCompute::VulkanCompute(ConsoleIO& io, bool debug)
 : io_(io) {
     io_s = &io_;

    setupDebug();
    initInstance();
    if (debug)
        initDebug();
    initDevice();

}

VulkanCompute::~VulkanCompute() {
    deInitDebug();
    deInitDevice();
    deInitInstance();
}

void
VulkanCompute::initInstance() {

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Amethyst VK Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 3);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = instance_layers_.size();
    createInfo.ppEnabledLayerNames = instance_layers_.data();
    createInfo.enabledExtensionCount = instance_extensions_.size();
    createInfo.ppEnabledExtensionNames = instance_extensions_.data();

    if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void
VulkanCompute::deInitInstance() {
    vkDestroyInstance(instance_, nullptr);
    instance_ = nullptr;
}

void
VulkanCompute::initDevice() {

    {
        // Physical Devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());
        io_.out << "available layers:" << std::endl;
        for (const auto& device : devices) {
            VkPhysicalDeviceProperties deviceProperties;
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            bool isGPU = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
            uint32_t queueFamCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFams(queueFamCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamCount, queueFams.data());

            bool isCompute = false;

            for (auto& qfam : queueFams) {
                isCompute = qfam.queueFlags & VK_QUEUE_COMPUTE_BIT;
                io_.out << "Queue is compute: " << isCompute << std::endl;
            }

            io_.out << "\t" << "isGPU: " << isGPU << std::endl;

            if (isCompute && isGPU)
                gpu_ = device;
        }
    }

    {
        // Enumerate Queue Family
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(gpu_, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(gpu_, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                graphics_family_index = i;
            }

            if (graphics_family_index != std::numeric_limits<uint32_t>::max()) {
                break;
            }

            i++;
        }

        if (graphics_family_index == std::numeric_limits<uint32_t>::max()) {
            throw std::runtime_error("Vulkan ERROR: Queue family supporting compute not found.");
        }
    }
    {
        // Enumerate Layers
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        io_.out << "available layers:" << std::endl;
        for (const auto& layer : availableLayers) {
            io_.out << "\t" << layer.layerName << "\t\t | " << layer.description << std::endl;
        }
    }

    float queue_priorities[] { 1.0f};

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphics_family_index;
    queueCreateInfo.queueCount       = 1;
    queueCreateInfo.pQueuePriorities = queue_priorities;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.enabledLayerCount = device_layers_.size();
    createInfo.ppEnabledLayerNames = device_layers_.data();
    createInfo.enabledExtensionCount = device_extensions_.size();
    createInfo.ppEnabledExtensionNames = device_extensions_.data();

    if (vkCreateDevice(gpu_, &createInfo, nullptr, &device_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device_, graphics_family_index, 0, &computeQueue_);

    // Compute command pool
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = graphics_family_index;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(vkCreateCommandPool(device_, &cmdPoolInfo, nullptr, &commandPool_));
}

void
VulkanCompute::deInitDevice() {
    vkDestroyDevice( device_, nullptr);
    device_ = VK_NULL_HANDLE;
}

void
VulkanCompute::testvk() {

    // Enumerate VK Extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    io_.out << "available extensions:" << std::endl;

    for (const auto& extension : extensions) {
        io_.out << "\t" << extension.extensionName << std::endl;
    }


}

void
VulkanCompute::setupDebug() {

    instance_layers_.push_back( "VK_LAYER_LUNARG_standard_validation");

    instance_extensions_.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );

    device_layers_.push_back( "VK_LAYER_LUNARG_standard_validation");

}

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanCompute::vulkanDebugCallback(
    VkDebugReportFlagsEXT      flags,
    VkDebugReportObjectTypeEXT obj_type,
    uint64_t                   src_obj,
    size_t                     location,
    int32_t                    msg_code,
    const char*                layer_prefix,
    const char*                msg,
    void *                     user_data
    )
{
    std::ostringstream ss;
    ss << "VKDBG: ";

    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        ss << "INFO: ";
    }
    if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        ss << "WARNING: ";
    }
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        ss << "PERFORMANCE: ";
    }
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        ss << "ERROR: ";
    }
    if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        ss << "DEBUG: ";
    }

    ss << "@[" << layer_prefix << "]: ";
    ss << msg << std::endl;

    io_s->out << ss.str();

#ifdef _WIN32
    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT || flags &
        flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        MessageBox(NULL, ss.str().c_str(), "Vulkan Error!", 0);
    }
#endif

    return false;
}


void
VulkanCompute::initDebug() {
    using namespace std::placeholders;

    fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance_, "vkCreateDebugReportCallbackEXT" );
    fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugReportCallbackEXT" );
    if (fvkCreateDebugReportCallbackEXT == nullptr || fvkDestroyDebugReportCallbackEXT == nullptr) {
        throw std::runtime_error("Vulkan ERROR: Can't fetch debug function pointers.");
    }

    VkDebugReportCallbackCreateInfoEXT debug_callback_create_info {};
    debug_callback_create_info.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug_callback_create_info.pfnCallback = VulkanCompute::vulkanDebugCallback;
    debug_callback_create_info.flags       =
        VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
        VK_DEBUG_REPORT_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_ERROR_BIT_EXT |
        VK_DEBUG_REPORT_DEBUG_BIT_EXT |
        0;

    fvkCreateDebugReportCallbackEXT(instance_, &debug_callback_create_info, nullptr, &debug_report_);
}

void
VulkanCompute::deInitDebug() {
    fvkDestroyDebugReportCallbackEXT(instance_, debug_report_, nullptr);
    fvkCreateDebugReportCallbackEXT = nullptr;
    fvkDestroyDebugReportCallbackEXT = nullptr;

}

VkDevice
VulkanCompute::getDevice() {
    return device_;
}

VkQueue
VulkanCompute::getQueue() {
    return computeQueue_;
}

} // namespace amethyst::lib