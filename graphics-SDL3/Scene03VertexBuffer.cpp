#include "Scene03VertexBuffer.h"

void Scene03VertexBuffer::Load(Renderer& renderer) 
{
    basePath = SDL_GetBasePath();
    vertexShader = renderer.LoadShader(basePath, "PositionColor.vert", 0, 0, 0, 0);
    fragmentShader = renderer.LoadShader(basePath, "SolidColor.frag", 0, 0, 0, 0);

    // Create the pipeline
    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .vertex_input_state = SDL_GPUVertexInputState{
            .vertex_buffer_descriptions = new SDL_GPUVertexBufferDescription[1]{{
                .slot = 0,
                .pitch = sizeof(PositionColorVertex),
                .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0,
            }},
            .num_vertex_buffers = 1,
            .vertex_attributes = new SDL_GPUVertexAttribute[2]{{
                .location = 0,
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset = 0
            }, {
                .location = 1,
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM,
                .offset = sizeof(float) * 3
            }},
            .num_vertex_attributes = 2,
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {
            .color_target_descriptions = new SDL_GPUColorTargetDescription[1]{{
                .format = SDL_GetGPUSwapchainTextureFormat(renderer.device, renderer.renderWindow)
            }},
            .num_color_targets = 1,
        },
    };
    pipeline = renderer.CreateGPUGraphicsPipeline(pipelineCreateInfo);
    if (pipeline == nullptr) {
        SDL_Log("Failed to create fill pipeline!");
    }

    // Clean up shader resources
    renderer.ReleaseShader(vertexShader);
    renderer.ReleaseShader(fragmentShader);

    // Create the vertex buffer
    SDL_GPUBufferCreateInfo vertexBufferCreateInfo = 
    {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(PositionColorVertex) * 6
    };
    vertexBuffer = renderer.CreateBuffer(vertexBufferCreateInfo);

    // Create the transfer buffer
    SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = 
    {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(PositionColorVertex) * 6
    };
    SDL_GPUTransferBuffer* transferBuffer = renderer.CreateTransferBuffer(transferBufferCreateInfo);

    // Map the transfer buffer and fill it with square data
    auto* transferData = static_cast<PositionColorVertex*>(
        renderer.MapTransferBuffer(transferBuffer, false)
        );

    // Define square vertices (Two triangles forming a square)
    transferData[0] = PositionColorVertex{ -0.5f, -0.5f, 0.0f, 255, 0, 197, 255 }; // Bottom left 
    transferData[1] = PositionColorVertex{ 0.5f, -0.5f, 0.0f, 216, 255, 0, 255 }; // Bottom right
    transferData[2] = PositionColorVertex{ -0.5f,  0.5f, 0.0f, 255, 147, 255, 255 }; // Top left 

    transferData[3] = PositionColorVertex{ 0.5f, -0.5f, 0.0f, 0, 255, 0, 255 }; // Bottom right
    transferData[4] = PositionColorVertex{ 0.5f,  0.5f, 0.0f, 255, 255, 0, 255 }; // Top right 
    transferData[5] = PositionColorVertex{ -0.5f,  0.5f, 0.0f, 0, 0, 255, 255 }; // Top left 

    renderer.UnmapTransferBuffer(transferBuffer);

    // Upload the transfer data to the vertex buffer
    SDL_GPUTransferBufferLocation transferBufferLocation = {
        .transfer_buffer = transferBuffer,
        .offset = 0
    };
    SDL_GPUBufferRegion vertexBufferRegion = {
        .buffer = vertexBuffer,
        .offset = 0,
        .size = sizeof(PositionColorVertex) * 6
    };
    renderer.BeginUploadToBuffer();
    renderer.UploadToBuffer(transferBufferLocation, vertexBufferRegion, false);
    renderer.EndUploadToBuffer(transferBuffer);
}

bool Scene03VertexBuffer::Update(float dt) 
{
    const bool isRunning = ManageInput(inputState);
    return isRunning;
}

void Scene03VertexBuffer::Draw(Renderer& renderer) 
{
    renderer.Begin();
    renderer.BindGraphicsPipeline(pipeline);
    SDL_GPUBufferBinding vertexBindings = { .buffer = vertexBuffer, .offset = 0 };
    renderer.BindVertexBuffers(0, vertexBindings, 1);
    renderer.DrawPrimitives(6, 1, 0, 0);
    renderer.End();
}

void Scene03VertexBuffer::Unload(Renderer& renderer) 
{
    renderer.ReleaseBuffer(vertexBuffer);
    renderer.ReleaseGraphicsPipeline(pipeline);
}
