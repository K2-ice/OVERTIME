#include "pch.h"
#include "Bullet.h"

namespace EngineL {
	Bullet::Bullet(float x, float y, float directionX, float directionY, float damage, sf::Color color, float width, float height, Map* map)
		: Entity(x, y, width, height, color) {

		this->directionX = directionX;
		this->directionY = directionY;
		this->damage = damage;
		this->map = map;
	}

	void Bullet::update(float deltaTime) {
		float moveX = directionX * speed * deltaTime;
		float moveY = directionY * speed * deltaTime;

		if (map != nullptr && map->isWallArea(getPosition().x + moveX, getPosition().y + moveY, 8.f, 8.f))
		{
			return;
		}

		move(moveX, moveY);
	}

	float Bullet::getDamage() const {
		return damage;
	}
}