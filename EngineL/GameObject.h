#pragma once
#include "Renderer.h"

namespace EngineL {

    class GameObject
    {
    public:
        virtual ~GameObject() {}

        virtual void update(float deltaTime) {}
        virtual void render(Renderer& renderer) {}
    };

}