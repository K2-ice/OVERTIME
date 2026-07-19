#include "pch.h"
#include "Bullet.h"

namespace EngineL
{
    Bullet::Bullet(float x, float y, float directionX, float directionY)
        : Entity(x, y, 8.f, 8.f, sf::Color::Yellow, 1)
    {
        this->directionX = directionX;
        this->directionY = directionY;
    }

    void Bullet::update(float deltaTime)
    {
        move(directionX * speed * deltaTime, directionY * speed * deltaTime);
    }
}