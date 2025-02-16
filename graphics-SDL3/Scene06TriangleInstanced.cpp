#include "Scene06TriangleInstanced.h"

void Scene06TriangleIndexed::Load(Renderer& renderer) 
{
	basePath = SDL_GetBasePath();
	vertexShader = renderer.LoadShader(basePath, "PositionColorInstanced.vert", 0, 0, 0,
		0);
	fragmentShader = renderer.LoadShader(basePath, "SolidColor.frag", 0, 0, 0, 0);

	// Create the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = 
	{
	.vertex_shader = vertexShader,
	.fragment_shader = fragmentShader,

	// This is set up to match the vertex shader layout!
	.vertex_input_state = SDL_GPUVertexInputState 
		{
	.vertex_buffer_descriptions = new SDL_GPUVertexBufferDescription[1] {{
	.slot = 0,
	.pitch = sizeof(PositionColorVertex),
	.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
	.instance_step_rate = 0,
	}},
	.num_vertex_buffers = 1,
	.vertex_attributes = new SDL_GPUVertexAttribute[2] 
	{{
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
	.color_target_descriptions = new SDL_GPUColorTargetDescription[1] 
	{{
	.format = SDL_GetGPUSwapchainTextureFormat(renderer.device,
	renderer.renderWindow)
	}},
	.num_color_targets = 1,
	},
	};
	pipeline = renderer.CreateGPUGraphicsPipeline(pipelineCreateInfo);

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

	// Create the index buffer
	SDL_GPUBufferCreateInfo indexBufferCreateInfo = 
	{
	.usage = SDL_GPU_BUFFERUSAGE_INDEX,
	.size = sizeof(Uint16) * 6
	};
	indexBuffer = renderer.CreateBuffer(indexBufferCreateInfo);

	// Set the buffer data
	SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = 
	{
	.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
	.size = (sizeof(PositionColorVertex) * 6) + (sizeof(Uint16) * 12),
	};
	SDL_GPUTransferBuffer* transferBuffer =
		renderer.CreateTransferBuffer(transferBufferCreateInfo);

	// Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
	auto transferData = static_cast<PositionColorVertex*>(
	renderer.MapTransferBuffer(transferBuffer, false)
	);
	/*
	transferData[0] = PositionColorVertex{ -1, -1, 0, 255, 0, 0, 255 };
	transferData[1] = PositionColorVertex{ 1, -1, 0, 0, 255, 0, 255 };
	transferData[2] = PositionColorVertex{ 0, 1, 0, 0, 0, 255, 255 }; 
	*/

	// Define square vertices (Two triangles forming a square)
	transferData[0] = PositionColorVertex{ -0.5f, -0.5f, 0.0f, 255, 0, 197, 255 }; // Bottom left 
	transferData[1] = PositionColorVertex{ 0.5f, -0.5f, 0.0f, 216, 255, 0, 255 }; // Bottom right
	transferData[2] = PositionColorVertex{ -0.5f,  0.5f, 0.0f, 255, 147, 255, 255 }; // Top left 

	transferData[3] = PositionColorVertex{ 0.5f, -0.5f, 0.0f, 255, 255, 255, 255 }; // Bottom right
	transferData[4] = PositionColorVertex{ 0.5f,  0.5f, 0.0f, 255, 255, 255, 255 }; // Top right 
	transferData[5] = PositionColorVertex{ -0.5f,  0.5f, 0.0f, 255, 255, 255, 255 }; // Top left 

	Uint16* indexData = reinterpret_cast<Uint16*>(&transferData[6]);
	for (Uint16 i = 0; i < 6; i += 1) 
	{
		indexData[i] = i +3; // draw from index 3
	}
	renderer.UnmapTransferBuffer(transferBuffer);
	renderer.BeginUploadToBuffer();

	// Upload the transfer data to the vertex and index buffer
	SDL_GPUTransferBufferLocation transferVertexBufferLocation
	{
	.transfer_buffer = transferBuffer,
	.offset = 0
	};
	SDL_GPUBufferRegion vertexBufferRegion
	{
	.buffer = vertexBuffer,
	.offset = 0,
	.size = sizeof(PositionColorVertex) * 6
	};
	SDL_GPUTransferBufferLocation transferIndexBufferLocation
	{
	.transfer_buffer = transferBuffer,
	.offset = sizeof(PositionColorVertex) * 6
	};
	SDL_GPUBufferRegion indexBufferRegion
	{
	.buffer = indexBuffer,
	.offset = 0,
	.size = sizeof(Uint16) * 6
	};

	renderer.UploadToBuffer(transferVertexBufferLocation, vertexBufferRegion, false);
	renderer.UploadToBuffer(transferIndexBufferLocation, indexBufferRegion, false);
	renderer.EndUploadToBuffer(transferBuffer);

	// Finally, print instructions!
	SDL_Log("Press Up to toggle the Index Buffer"); 
	SDL_Log("Using index buffer: %s", useIndexBuffer ? "true" : "false");
}

bool Scene06TriangleIndexed::Update(float dt) 
{
	const bool isRunning = ManageInput(inputState);
	if (inputState.IsPressed(DirectionalKey::Up))
	{
		useIndexBuffer = !useIndexBuffer;
		SDL_Log("Using index buffer: %s", useIndexBuffer ? "true" : "false");
	}
	return isRunning;
}

void Scene06TriangleIndexed::Draw(Renderer& renderer) 
{
	renderer.Begin();
	renderer.BindGraphicsPipeline(pipeline);
	SDL_GPUBufferBinding vertexBindings = { .buffer = vertexBuffer, .offset = 0 };
	renderer.BindVertexBuffers(0, vertexBindings, 1);
	if (useIndexBuffer) 
	{
		SDL_GPUBufferBinding indexBindings = { .buffer = indexBuffer, .offset = 0 };
		renderer.BindIndexBuffer(indexBindings, SDL_GPU_INDEXELEMENTSIZE_16BIT);
		renderer.DrawIndexedPrimitives(6, 16, 0, 0, 0);
	}
	else 
	{
		renderer.DrawPrimitives(6, 16, 0, 0);
	}
	renderer.End();
}

void Scene06TriangleIndexed::Unload(Renderer& renderer) 
{
	renderer.ReleaseBuffer(vertexBuffer);
	renderer.ReleaseBuffer(indexBuffer);
	renderer.ReleaseGraphicsPipeline(pipeline);
}
