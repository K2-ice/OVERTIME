#pragma once
#include "GameObject.h"
#include <vector>

namespace EngineL
{
    class UpdateManager
    {
    public:
        void add(GameObject* object);
        void updateAll(float deltaTime);
        void remove(GameObject* object);
    private:
        std::vector<GameObject*> objects;
    };
}