#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
struct QueueFamilyIndices
{
    int graphicsFamily = -1; // Location of Graphics Queue Family
    int presentationFamily = -1; // Location of Presentation Queue Family
    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << '\n';
    return VK_FALSE;
}

const std::vector<const char*> deviceExtensions
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
