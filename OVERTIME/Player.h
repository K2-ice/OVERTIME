#pragma once
#include "Entity.h"
#include "InputManager.h"

namespace EngineL
{
    class Player : public Entity
    {
    public:
        Player(float x, float y, InputManager* inputManager);

        void update(float deltaTime) override;

        void aimAt(float mouseX, float mouseY);

        bool wantsToShoot() const;
        void resetShootCooldown();

        float getShootDirectionX() const;
        float getShootDirectionY() const;

    private:
        InputManager* inputManager;
        float speed = 200.f;

        float shootDirectionX;
        float shootDirectionY;

        float shootCooldown = 0.f;
        float shootCooldownMax = 0.2f;
    };
}