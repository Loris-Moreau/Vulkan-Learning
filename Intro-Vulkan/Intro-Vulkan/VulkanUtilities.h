﻿#pragma once

#include <iostream>

struct QueueFamilyIndices
{
    int graphicsFamily = -1; // Location of Graphics Queue Family
    bool isValid()
    {
        return graphicsFamily >= 0;
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
