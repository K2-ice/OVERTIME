#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

namespace EngineL
{
    class Entity : public GameObject
    {
    public:
        Entity(float x, float y, float width, float height, sf::Color color, int maxHealth);

        void update(float deltaTime) override;
        void render(Renderer& renderer) override;

        void setPosition(float x, float y);
        sf::Vector2f getPosition() const;
        void move(float offsetX, float offsetY);

        void takeDamage(int amount);
        bool isAlive() const;
        int getHealth() const;

    protected:
        sf::RectangleShape shape;
        int health;
        int maxHealth;
    };
}