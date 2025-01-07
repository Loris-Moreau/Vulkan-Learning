#pragma once

#include "Scene.h"

#include <SDL3/SDL.h>

class Scene03VertexBuffer : public Scene 
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
	SDL_GPUGraphicsPipeline* pipeline;
	SDL_GPUBuffer* vertexBuffer;
};
