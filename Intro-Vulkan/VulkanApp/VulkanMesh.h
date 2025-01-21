#pragma once

#define GLFW_INCLUDE_VULKAN

#include "VulkanUtilities.h"
#include <GLFW/glfw3.h>
#include <vector>
using std::vector;

struct Model
{
	glm::mat4 model;
};

class VulkanMesh
{
public:
	VulkanMesh(vk::PhysicalDevice physicalDeviceP,vk::Device deviceP,
		vk::Queue transferQueue, vk::CommandPool transferCommandPool, vector<Vertex>* vertices,
		vector<uint32_t>* indices);
	VulkanMesh() = default;
	~VulkanMesh() = default;
	size_t getVextexCount();
	vk::Buffer getVertexBuffer();
	void destroyBuffers();

	size_t getIndexCount();

	vk::Buffer getIndexBuffer();
	
	Model getModel() const { return model; }
	void setModel(const glm::mat4& modelP) { model.model = modelP; }
	
private:
	size_t vertexCount;
	vk::Buffer vertexBuffer;
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::DeviceMemory vertexBufferMemory;
	void createVertexBuffer(vk::Queue transferQueue, vk::CommandPool transferCommandPool, vector<Vertex>* vertices);
	uint32_t findMemoryTypeIndex(uint32_t allowedTypes, vk::MemoryPropertyFlags properties);

	uint32_t findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t allowedTypes, vk::MemoryPropertyFlags properties);

	size_t indexCount;
	vk::Buffer indexBuffer;
	vk::DeviceMemory indexBufferMemory;
	
	void createIndexBuffer(vk::Queue transferQueue, vk::CommandPool transferCommandPool, vector<uint32_t>* indices);

	Model model;
};
