#pragma once

#include "Scene.h"

class Scene02Triangle : public Scene 
{
public:
	void Load(Renderer& renderer) override;
	bool Update(float dt) override;
	void Draw(Renderer& renderer) override;
	void Unload(Renderer& renderer) override;

private:
	InputState inputState;
	const char* basePath;
	SDL_GPUShader* vertexShader;
	SDL_GPUShader* fragmentShader;

	SDL_GPUGraphicsPipeline* fillPipeline;
	SDL_GPUGraphicsPipeline* linePipeline;

	SDL_GPUViewport smallViewport = { 256, 190, 512, 380, 0.1f, 1.0f };
	SDL_Rect scissorRect = { 512, 380, 512, 380 };

	bool useWireframeMode = false;
	bool useSmallViewport = false;
	bool useScissorRect = false;
};
