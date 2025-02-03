#pragma once

#include "SphereCubeMaterial.hpp"
#include "SphereCubeMesh.hpp"
#include "Mat4.hpp"
#include "Drawable.hpp"

using gmath::Mat4;

namespace gdraw
{
    class SphereCube : public Drawable
    {
    public:
        explicit SphereCube(Renderer& renderer_) : material(&renderer_), mesh(&renderer_) {}

        void Load();
        void Update(f32 dt);
        void Draw(const Renderer& renderer);
        void Unload();

    private:
        SphereCubeMaterial material;
        SphereCubeMesh mesh;
        Mat4 transform { Mat4::identity };

        void Bind();
    };
}
