#include "pch.h"
#include "Bullet.h"

namespace EngineL {

	Bullet::Bullet(float x, float y, float directionX, float directionY, float damage, sf::Color color, float width, float height, Map* map)
		: Entity(x, y, width, height, color) {

		this->directionX = directionX;
		this->directionY = directionY;
		this->damage = damage;
		this->map = map;
		this->width = width;
		this->height = height;
	}

	void Bullet::update(float deltaTime) {

		if (hitWall)

			return;

		float moveX = directionX * speed * deltaTime;
		float moveY = directionY * speed * deltaTime;

		if (map != nullptr && map->isWallArea(getPosition().x + moveX, getPosition().y + moveY, width, height)) {
			hitWall = true;
			return;
		}

		move(moveX, moveY);
	}

	bool Bullet::hasHitWall() const {
		return hitWall;
	}

	float Bullet::getDamage() const {
		return damage;
	}
}