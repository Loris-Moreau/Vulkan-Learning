#pragma once

#define GLFW_INCLUDE_VULKAN
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
};
