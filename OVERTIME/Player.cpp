#include "pch.h"
#include "Player.h"
#include <cmath>

namespace EngineL
{
    Player::Player(float x, float y, InputManager* inputManager)
        : Entity(x, y, 32.f, 32.f, sf::Color::Blue, 100)
    {
        this->inputManager = inputManager;
        shootDirectionX = 0.f;
        shootDirectionY = 0.f;
    }

    void Player::update(float deltaTime)
    {
        float dx = 0.f;
        float dy = 0.f;

        if (inputManager->isMovingUp())
            dy -= 1.f;
        if (inputManager->isMovingDown())
            dy += 1.f;
        if (inputManager->isMovingLeft())
            dx -= 1.f;
        if (inputManager->isMovingRight())
            dx += 1.f;

        move(dx * speed * deltaTime, dy * speed * deltaTime);

        if (shootCooldown > 0.f)
            shootCooldown -= deltaTime;
    }

    void Player::aimAt(float mouseX, float mouseY)
    {
        float playerX = getPosition().x;
        float playerY = getPosition().y;

        float dirX = mouseX - playerX;
        float dirY = mouseY - playerY;

        float length = std::sqrt(dirX * dirX + dirY * dirY);

        if (length != 0.f)
        {
            shootDirectionX = dirX / length;
            shootDirectionY = dirY / length;
        }
    }

    bool Player::wantsToShoot() const
    {
        return inputManager->isMouseButtonPressed(sf::Mouse::Button::Left) && shootCooldown <= 0.f;
    }

    void Player::resetShootCooldown()
    {
        shootCooldown = shootCooldownMax;
    }

    float Player::getShootDirectionX() const
    {
        return shootDirectionX;
    }

    float Player::getShootDirectionY() const
    {
        return shootDirectionY;
    }
}