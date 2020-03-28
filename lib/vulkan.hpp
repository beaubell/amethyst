#ifndef _AMETHYST_LIB_VULKAN_HPP
#define _AMETHYST_LIB_VULKAN_HPP

#include <vulkan/vulkan.hpp>
#include <vector>

#include "console_io.h"
namespace amethyst::lib {

class VulkanCompute {
  public:
    VulkanCompute(ConsoleIO& io, bool debug);
    virtual ~VulkanCompute();

    auto testvk() -> void;

  private:
    auto initInstance() -> void;
    auto deInitInstance() -> void;

    auto setupDebug() -> void;
    auto initDebug() -> void;
    auto deInitDebug() -> void;

    auto initDevice() -> void;
    auto deInitDevice() -> void;

    static VKAPI_ATTR VkBool32 VKAPI_CALL
        vulkanDebugCallback(VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char*, void *);

    ConsoleIO& io_;
    static ConsoleIO* io_s;

    VkInstance       instance_ = VK_NULL_HANDLE;
    VkPhysicalDevice gpu_      = VK_NULL_HANDLE;
    VkDevice         device_   = VK_NULL_HANDLE;
    VkPhysicalDevice gpu_properties_ = {};

    uint32_t         graphics_family_index = std::numeric_limits<uint32_t>::max();

    std::vector<const char*> instance_layers_;
    std::vector<const char*> instance_extensions_;
    std::vector<const char*> device_layers_;
    std::vector<const char*> device_extensions_;

    VkDebugReportCallbackEXT debug_report_ = VK_NULL_HANDLE;

    VkQueue          computeQueue_;


};

} //  namespace amethyst::lib

#endif
