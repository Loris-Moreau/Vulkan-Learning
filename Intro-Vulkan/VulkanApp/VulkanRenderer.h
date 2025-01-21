#pragma once

#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
#include <vector>
#include <GLFW/glfw3.h>

using std::vector;
#include <set>
using std::set;
#include <array>
using std::array;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanMesh.h"
#include "VulkanUtilities.h"

struct
{
	vk::PhysicalDevice physicalDevice;
	vk::Device logicalDevice;
} mainDevice;

struct MVP
{
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
};

struct ViewProjection
{
	glm::mat4 projection;
	glm::mat4 view;
};

class VulkanRenderer
{
public:
#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif
	static const vector<const char*> validationLayers;

	VulkanRenderer();
	~VulkanRenderer();

	int init(GLFWwindow* windowP);
	void draw();
	void clean();
	
private:
	GLFWwindow* window;
	vk::Instance instance;
	vk::Queue graphicsQueue;			// Handles to queue (no value stored)
	VkDebugUtilsMessengerEXT debugMessenger;
	
	vk::SurfaceKHR surface;
	vk::Queue presentationQueue;
	vk::SwapchainKHR swapchain;
	vk::Format swapchainImageFormat;
	vk::Extent2D swapchainExtent;
	vector<SwapchainImage> swapchainImages;

	vk::PipelineLayout pipelineLayout;
	vk::RenderPass renderPass;
	vk::Pipeline graphicsPipeline;

	vector<vk::Framebuffer> swapchainFramebuffers;
	vk::CommandPool graphicsCommandPool;
	vector<vk::CommandBuffer> commandBuffers;

	vector<vk::Semaphore> imageAvailable;
	vector<vk::Semaphore> renderFinished;
	// Should be less than the number of swap-chain images, here 3 (could cause bugs)
	const int MAX_FRAME_DRAWS = 2;
	int currentFrame = 0;
	vector<vk::Fence> drawFences;

	// Instance
	void createInstance();
	bool checkInstanceExtensionSupport(const vector<const char*>& checkExtensions);
	bool checkValidationLayerSupport();
	vector<const char*> getRequiredExtensions();

	// Debug
	void setupDebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	// Devices
	void getPhysicalDevice();
	bool checkDeviceSuitable(vk::PhysicalDevice device);
	QueueFamilyIndices getQueueFamilies(vk::PhysicalDevice device);
	void createLogicalDevice();

	// Surface and swapchain
	vk::SurfaceKHR createSurface();
	bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
	SwapchainDetails getSwapchainDetails(vk::PhysicalDevice device);
	void createSwapchain();
	vk::SurfaceFormatKHR chooseBestSurfaceFormat(const vector<vk::SurfaceFormatKHR>& formats);
	vk::PresentModeKHR chooseBestPresentationMode(const vector<vk::PresentModeKHR>& presentationModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
	vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlagBits aspectFlags);

	// Graphics pipeline
	void createGraphicsPipeline();
	vk::ShaderModule createShaderModule(const vector<char>& code);
	void createRenderPass();

	// Buffers
	void createFramebuffers();
	void createGraphicsCommandPool();
	void createGraphicsCommandBuffers();
	void recordCommands();

	// Draw
	void createSynchronisation();

	VulkanMesh firstMesh;

	vector<VulkanMesh> meshes;

	MVP mvp;

	vk::DescriptorSetLayout descriptorSetLayout;
	void createDescriptorSetLayout();
	
	vector<vk::Buffer> vpUniformBuffer;
	vector<vk::DeviceMemory> vpUniformBufferMemory;
	void createUniformBuffers();

	vk::DescriptorPool descriptorPool;
	void createDescriptorPool();

	vector<vk::DescriptorSet> descriptorSets;
	void createDescriptorSets();

	void updateUniformBuffers(uint32_t imageIndex);

public:
	void updateModel(int modelId, glm::mat4 modelP);

private:
	ViewProjection viewProjection;
	
	VkDeviceSize minUniformBufferOffet;
	size_t modelUniformAlignement;
	//UboModel* modelTransferSpace;
	Model* modelTransferSpace;

	const int MAX_OBJECTS = 2;

	void allocateDynamicBufferTransferSpace();

	vector<vk::Buffer> modelUniformBufferDynamic;
	vector<vk::DeviceMemory> modelUniformBufferMemoryDynamic;
};
