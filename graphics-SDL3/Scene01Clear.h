#pragma once

#include "Scene.h"

#include <SDL3/SDL_events.h>

class Scene01Clear : public Scene
{
public:
    void Load(Renderer& renderer) override;
    bool Update(float dt) override;
    void Draw(Renderer& renderer) override;
    void Unload(Renderer& renderer) override;
    
private:
    InputState inputState;
};