#pragma once
#include "GameObject.h"
#include "Renderer.h"
#include <vector>

namespace EngineL
{
    class RenderManager
    {
    public:
        void add(GameObject* object);
        void renderAll(Renderer& renderer);
        void remove(GameObject* object);

    private:
        std::vector<GameObject*> objects;
    };
}