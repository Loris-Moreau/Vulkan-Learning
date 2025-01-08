#include "Scene09Cube.h"
#include <SDL3/SDL.h>
#include "PositionColorVertex.h"
#include "Renderer.h"
#include "PositionTextureVertex.h"

typedef struct CubeVertex
{
	float x, y, z; /* 3D data. Vertex range -0.5..0.5 in all axes. Z -0.5 is near, 0.5 is far. */
	float red, green, blue;  /* intensity 0 to 1 (alpha is always 1). */
} VertexData;

static const CubeVertex cubeVertices[] = {
	/* Front face. */
	/* Bottom left */
	{ -0.5,  0.5, -0.5, 1.0, 0.0, 0.0 }, /* red */
	{  0.5, -0.5, -0.5, 0.0, 0.0, 1.0 }, /* blue */
	{ -0.5, -0.5, -0.5, 0.0, 1.0, 0.0 }, /* green */

	/* Top right */
	{ -0.5, 0.5, -0.5, 1.0, 0.0, 0.0 }, /* red */
	{ 0.5,  0.5, -0.5, 1.0, 1.0, 0.0 }, /* yellow */
	{ 0.5, -0.5, -0.5, 0.0, 0.0, 1.0 }, /* blue */

	/* Left face */
	/* Bottom left */
	{ -0.5,  0.5,  0.5, 1.0, 1.0, 1.0 }, /* white */
	{ -0.5, -0.5, -0.5, 0.0, 1.0, 0.0 }, /* green */
	{ -0.5, -0.5,  0.5, 0.0, 1.0, 1.0 }, /* cyan */

	/* Top right */
	{ -0.5,  0.5,  0.5, 1.0, 1.0, 1.0 }, /* white */
	{ -0.5,  0.5, -0.5, 1.0, 0.0, 0.0 }, /* red */
	{ -0.5, -0.5, -0.5, 0.0, 1.0, 0.0 }, /* green */

	/* Top face */
	/* Bottom left */
	{ -0.5, 0.5,  0.5, 1.0, 1.0, 1.0 }, /* white */
	{  0.5, 0.5, -0.5, 1.0, 1.0, 0.0 }, /* yellow */
	{ -0.5, 0.5, -0.5, 1.0, 0.0, 0.0 }, /* red */

	/* Top right */
	{ -0.5, 0.5,  0.5, 1.0, 1.0, 1.0 }, /* white */
	{  0.5, 0.5,  0.5, 0.0, 0.0, 0.0 }, /* black */
	{  0.5, 0.5, -0.5, 1.0, 1.0, 0.0 }, /* yellow */

	/* Right face */
	/* Bottom left */
	{ 0.5,  0.5, -0.5, 1.0, 1.0, 0.0 }, /* yellow */
	{ 0.5, -0.5,  0.5, 1.0, 0.0, 1.0 }, /* magenta */
	{ 0.5, -0.5, -0.5, 0.0, 0.0, 1.0 }, /* blue */

	/* Top right */
	{ 0.5,  0.5, -0.5, 1.0, 1.0, 0.0 }, /* yellow */
	{ 0.5,  0.5,  0.5, 0.0, 0.0, 0.0 }, /* black */
	{ 0.5, -0.5,  0.5, 1.0, 0.0, 1.0 }, /* magenta */

	/* Back face */
	/* Bottom left */
	{  0.5,  0.5, 0.5, 0.0, 0.0, 0.0 }, /* black */
	{ -0.5, -0.5, 0.5, 0.0, 1.0, 1.0 }, /* cyan */
	{  0.5, -0.5, 0.5, 1.0, 0.0, 1.0 }, /* magenta */

	/* Top right */
	{  0.5,  0.5,  0.5, 0.0, 0.0, 0.0 }, /* black */
	{ -0.5,  0.5,  0.5, 1.0, 1.0, 1.0 }, /* white */
	{ -0.5, -0.5,  0.5, 0.0, 1.0, 1.0 }, /* cyan */

	/* Bottom face */
	/* Bottom left */
	{ -0.5, -0.5, -0.5, 0.0, 1.0, 0.0 }, /* green */
	{  0.5, -0.5,  0.5, 1.0, 0.0, 1.0 }, /* magenta */
	{ -0.5, -0.5,  0.5, 0.0, 1.0, 1.0 }, /* cyan */

	/* Top right */
	{ -0.5, -0.5, -0.5, 0.0, 1.0, 0.0 }, /* green */
	{  0.5, -0.5, -0.5, 0.0, 0.0, 1.0 }, /* blue */
	{  0.5, -0.5,  0.5, 1.0, 0.0, 1.0 } /* magenta */
};

Uint32 vertsSize = sizeof(cubeVertices);
Uint32 vertsNum = vertsSize / sizeof(CubeVertex);

void Scene09Cube::Load()
{
	auto basePath = SDL_GetBasePath();
	vertexShader = renderer.LoadShader(basePath, "TexturedQuadWithMatrix.vert", 0, 1, 0,
		0);
	fragmentShader = renderer.LoadShader(basePath, "TexturedQuadWithMultiplyColor.frag",
		1, 1, 0, 0);
	SDL_Surface* imageData = renderer.LoadBMPImage(basePath, "cube0.bmp", 4);
	if (imageData == nullptr)
	{
		SDL_Log("Could not load image data!");
	}
	// Create the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
		// This is set up to match the vertex shader layout!
		.vertex_input_state = SDL_GPUVertexInputState{
			.vertex_buffer_descriptions = new SDL_GPUVertexBufferDescription[1]{{
				.slot = 0,
				.pitch = sizeof(CubeVertex),
				.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
				.instance_step_rate = 0,
			}},
			.num_vertex_buffers = 1,
			.vertex_attributes = new SDL_GPUVertexAttribute[2]{{.location = 0,
																.buffer_slot = 0,
																.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
																.offset = 0},
															   {.location = 1,
																.buffer_slot = 0,
																.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
																.offset = sizeof(float) * 3}},
			.num_vertex_attributes = 2,
		},

		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.target_info = {
			.color_target_descriptions = new SDL_GPUColorTargetDescription[1]{{.format = SDL_GetGPUSwapchainTextureFormat(renderer.device, renderer.renderWindow),
			.blend_state = {
				.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.color_blend_op = SDL_GPU_BLENDOP_ADD,
				.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
				.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
				.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
				.enable_blend = true,
			}}},
			.num_color_targets = 1,
		},
	};
	pipeline = renderer.CreateGPUGraphicsPipeline(pipelineCreateInfo);
	// Clean up shader resources
	renderer.ReleaseShader(vertexShader);
	renderer.ReleaseShader(fragmentShader);
	// Texture sampler
	sampler = renderer.CreateSampler(SDL_GPUSamplerCreateInfo{
		.min_filter = SDL_GPU_FILTER_NEAREST,
		.mag_filter = SDL_GPU_FILTER_NEAREST,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		});



	// Create the vertex buffer

	SDL_GPUBufferCreateInfo vertexBufferCreateInfo = {
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = vertsSize };
	vertexBuffer = renderer.CreateBuffer(vertexBufferCreateInfo);
	renderer.SetBufferName(vertexBuffer, "Ravioli Vertex Buffer");

	// Create texture
	SDL_GPUTextureCreateInfo textureInfo{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = static_cast<Uint32>(imageData->w),
		.height = static_cast<Uint32>(imageData->h),
		.layer_count_or_depth = 1,
		.num_levels = 1,
	};
	texture = renderer.CreateTexture(textureInfo);
	renderer.SetTextureName(texture, "Ravioli Texture");

	// Set the buffer data
	SDL_GPUTransferBufferCreateInfo transferBufferCreateInfo = {
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = (vertsSize)+(sizeof(Uint16) * 6),
	};
	SDL_GPUTransferBuffer* transferBuffer =
		renderer.CreateTransferBuffer(transferBufferCreateInfo);

	auto transferData = static_cast<CubeVertex*>(
		renderer.MapTransferBuffer(transferBuffer, false));

	for (int i = 0; i < vertsNum; i++) {
		transferData[i] = cubeVertices[i];
	}


	renderer.UnmapTransferBuffer(transferBuffer);

	// Setup texture transfer buffer
	Uint32 bufferSize = imageData->w * imageData->h * 4;
	SDL_GPUTransferBufferCreateInfo textureTransferBufferCreateInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = bufferSize };
	SDL_GPUTransferBuffer* textureTransferBuffer =
		renderer.CreateTransferBuffer(textureTransferBufferCreateInfo);
	auto textureTransferData = static_cast<CubeVertex*>(
		renderer.MapTransferBuffer(textureTransferBuffer, false));
	std::memcpy(textureTransferData, imageData->pixels, bufferSize);
	renderer.UnmapTransferBuffer(textureTransferBuffer);
	renderer.BeginUploadToBuffer();

	// Upload the transfer data to the vertex and index buffer
	SDL_GPUTransferBufferLocation transferVertexBufferLocation{
		.transfer_buffer = transferBuffer,
		.offset = 0 };
	SDL_GPUBufferRegion vertexBufferRegion{
		.buffer = vertexBuffer,
		.offset = 0,
		.size = vertsSize };

	SDL_GPUTextureTransferInfo textureBufferLocation{
		.transfer_buffer = textureTransferBuffer,
		.offset = 0 };

	SDL_GPUTextureRegion textureBufferRegion{
		.texture = texture,
		.w = static_cast<Uint32>(imageData->w),
		.h = static_cast<Uint32>(imageData->h),
		.d = 1 };
	renderer.UploadToBuffer(transferVertexBufferLocation, vertexBufferRegion, false);
	renderer.UploadToTexture(textureBufferLocation, textureBufferRegion, false);
	renderer.EndUploadToBuffer(transferBuffer);
	renderer.ReleaseTransferBuffer(textureTransferBuffer);
	renderer.ReleaseSurface(imageData);
}


void Scene09Cube::Unload()
{
	renderer.ReleaseSampler(sampler);
	renderer.ReleaseBuffer(vertexBuffer);
	renderer.ReleaseTexture(texture);
	renderer.ReleaseGraphicsPipeline(pipeline);
}

void Scene09Cube::Draw(Mat4 matrixUniform, FragMultiplyUniformCube fragMultiplyUniform0)
{

	renderer.BindGraphicsPipeline(pipeline);
	SDL_GPUBufferBinding vertexBindings{ .buffer = vertexBuffer, .offset = 0 };
	renderer.BindVertexBuffers(0, vertexBindings, 1);
	SDL_GPUTextureSamplerBinding textureSamplerBinding{ .texture = texture, .sampler = sampler };
	renderer.BindFragmentSamplers(0, textureSamplerBinding, 1);

	renderer.PushVertexUniformData(0, &matrixUniform, sizeof(matrixUniform));

	renderer.PushFragmentUniformData(0, &fragMultiplyUniform0,
		sizeof(FragMultiplyUniformCube));

	renderer.DrawPrimitives(vertsNum, 1, 0, 0);	
}
