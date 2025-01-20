#pragma once

#define GLFW_INCLUDE_VULKAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdexcept>
#include <vector>
#include <GLFW/glfw3.h>

#include "VulkanUtilities.h"
#include "vulkan/vulkan.hpp"

using std::vector;

struct 
{
	vk::PhysicalDevice physicalDevice;
	vk::Device logicalDevice;
} mainDevice;

class VulkanRenderer
{
public:
#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif
	static const vector<const char*> validationLayers;
	
public:
	VulkanRenderer();
	~VulkanRenderer();

	int init(GLFWwindow* windowP);
	bool checkInstanceExtensionSupport(const vector<const char*>& checkExtensions);
	void clean();
	
private:
	 
	GLFWwindow* window;
	vk::Instance instance;
	void createInstance();

	void getPhysicalDevice();
	bool checkDeviceSuitable(vk::PhysicalDevice device);
	QueueFamilyIndices getQueueFamilies(vk::PhysicalDevice device);

	void createLogicalDevice();

	vk::Queue graphicsQueue;

	bool checkValidationLayerSupport();

	vector<const char*> getRequiredExtensions();

	VkDebugUtilsMessengerEXT debugMessenger;

	VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	
	void setupDebugMessenger();


	
	vk::SurfaceKHR surface;
	vk::Queue presentationQueue;
	
	vk::SurfaceKHR createSurface();

	bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

	
	SwapchainDetails getSwapchainDetails(vk::PhysicalDevice device);
	vk::SwapchainKHR swapchain;
	void createSwapchain();
	vk::SurfaceFormatKHR chooseBestSurfaceFormat(const vector<vk::SurfaceFormatKHR>& formats);
	vk::PresentModeKHR chooseBestPresentationMode(const vector<vk::PresentModeKHR>& presentationModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
	vk::Format swapchainImageFormat;
	vk::Extent2D swapchainExtent;
	vector<SwapchainImage> swapchainImages;
	vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlagBits aspectFlags);
};
