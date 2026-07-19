#include "pch.h"
#include "Entity.h"

namespace EngineL
{
    Entity::Entity(float x, float y, float width, float height, sf::Color color, int maxHealth)
    {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(sf::Vector2f(x, y));
        shape.setFillColor(color);

        this->maxHealth = maxHealth;
        health = maxHealth;
    }

    void Entity::update(float deltaTime)
    {
    }

    void Entity::render(Renderer& renderer)
    {
        renderer.drawRectangle(shape);
    }

    void Entity::setPosition(float x, float y)
    {
        shape.setPosition(sf::Vector2f(x, y));
    }

    sf::Vector2f Entity::getPosition() const
    {
        return shape.getPosition();
    }

    void Entity::move(float offsetX, float offsetY)
    {
        shape.move(sf::Vector2f(offsetX, offsetY));
    }

    void Entity::takeDamage(int amount)
    {
        health -= amount;

        if (health < 0)
            health = 0;
    }

    bool Entity::isAlive() const
    {
        return health > 0;
    }

    int Entity::getHealth() const
    {
        return health;
    }
}