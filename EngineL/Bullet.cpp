#include "pch.h"
#include "Bullet.h"
namespace EngineL
{
	Bullet::Bullet(float x, float y, float directionX, float directionY, float damage, sf::Color color, float width, float height)
		: Entity(x, y, width, height, color)
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