#pragma once

#include "PositionCTVertex.h"
#include "Quad.h"
#include "Scene.h"

#include <array>
#include <string>
using std::array;
using std::string;

class Scene08TextureQuadMoving : public Scene 
{
public:
	void Load(Renderer& renderer) override;
	bool Update(float dt) override;
	void Draw(Renderer& renderer) override;
	void Unload(Renderer& renderer) override;

private:
	InputState inputState;

	const char* basePath{ nullptr };

	SDL_GPUShader* vertexShader{ nullptr };
	SDL_GPUShader* fragmentShader{ nullptr };

	SDL_GPUGraphicsPipeline* pipeline{ nullptr };

	SDL_GPUBuffer* vertexBuffer{ nullptr };
	SDL_GPUBuffer* indexBuffer{ nullptr };

	SDL_GPUTexture* texture{ nullptr };

	SDL_GPUSampler* sampler{ nullptr };

	float time{ 0 };
};
