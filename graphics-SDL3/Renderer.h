#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_log.h>

#include <vector>
#include <string>

using std::vector;
using std::string;

#include "Window.h"

class Window;

class Renderer
{
public:
    void Init(Window &window);
    void Begin(SDL_GPUDepthStencilTargetInfo* depthStencilTargetInfo = nullptr);
    void End() const;
    void Close() const;
    
    SDL_GPUDevice* device {nullptr};
    SDL_Window* renderWindow {nullptr};
    SDL_GPUCommandBuffer* cmdBuffer {nullptr};
    SDL_GPUTexture* swapchainTexture {nullptr};
    SDL_GPURenderPass* renderPass {nullptr};
};
