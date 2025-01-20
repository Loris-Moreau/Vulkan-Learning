#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vulkan/vulkan.hpp>

#include <vector>
using std::vector;
#include <string>
using std::string;

struct QueueFamilyIndices
{
    int graphicsFamily = -1; // Location of Graphics Queue Family
    int presentationFamily = -1; // Location of Presentation Queue Family
    
    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback (VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << '\n';
    return VK_FALSE;
}

const vector<const char*> deviceExtensions
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapchainDetails
{
    // What the surface is capable of displaying, e.g. image size/extent
    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    // Vector of the image formats, e.g. RGBA
    vector<vk::SurfaceFormatKHR> formats;
    // Vector of presentation modes
    vector<vk::PresentModeKHR> presentationModes;
};

struct SwapchainImage
{
    vk::Image image;
    vk::ImageView imageView;
};
