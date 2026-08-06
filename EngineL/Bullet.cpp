#include "pch.h"
#include "Bullet.h"

namespace EngineL
{
    Bullet::Bullet(float x, float y, float directionX, float directionY, float damage)
        : Entity(x, y, 8.f, 8.f, sf::Color::Yellow)
    {
        this->directionX = directionX;
        this->directionY = directionY;
        this->damage = damage;
    }

    void Bullet::update(float deltaTime)
    {
        move(directionX * speed * deltaTime, directionY * speed * deltaTime);
    }

    float Bullet::getDamage() const
    {
        return damage;
    }
}