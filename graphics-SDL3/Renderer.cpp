#include "Renderer.h"

void Renderer::Init(Window &window)
{
    renderWindow = window.sdlWindow;
    device = SDL_CreateGPUDevice(
    SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
    true,
    nullptr);
    SDL_ClaimWindowForGPUDevice(device, renderWindow);
}

void Renderer::Close() const
{
    SDL_ReleaseWindowFromGPUDevice(device, renderWindow);
    SDL_DestroyGPUDevice(device);
}

void Renderer::Begin(SDL_GPUDepthStencilTargetInfo* depthStencilTargetInfo)
{
    cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
    if (cmdBuffer == nullptr)
        {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
    }
    SDL_GPUTexture* swapchainTexture;

    if (!SDL_AcquireGPUSwapchainTexture(cmdBuffer, renderWindow, &swapchainTexture,
    nullptr, nullptr))
        {
        SDL_Log("AcquireGPUSwapchainTexture failed: %s", SDL_GetError());
    }
    if (swapchainTexture != nullptr)
        {
        SDL_GPUColorTargetInfo colorTargetInfo = {};
        colorTargetInfo.texture = swapchainTexture;
        colorTargetInfo.clear_color = SDL_FColor { 0.392f, 0.584f, 0.929f, 1.0f };
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        renderPass = SDL_BeginGPURenderPass(cmdBuffer, &colorTargetInfo, 1,
        depthStencilTargetInfo);
    }
}
void Renderer::End() const
{
    SDL_EndGPURenderPass(renderPass);
    SDL_SubmitGPUCommandBuffer(cmdBuffer);
}
