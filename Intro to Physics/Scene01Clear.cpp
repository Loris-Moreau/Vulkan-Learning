#include "Scene01Clear.h"

void Scene01Clear::Load(Renderer& renderer) 
{
}

bool Scene01Clear::Update(float dt) 
{
    return ManageInput(inputState);
}

void Scene01Clear::Draw(Renderer& renderer) 
{
    renderer.Begin();
    renderer.End();
}

void Scene01Clear::Unload(Renderer& renderer) 
{
}
