#include "pch.h"
#include "Bullet.h"

namespace EngineL {

	Bullet::Bullet(float x, float y, float directionX, float directionY, float damage, Map* map, sf::Color color, float size)
		: Entity(x, y, size, size, color) {

		this->directionX = directionX;
		this->directionY = directionY;
		this->damage = damage;
		this->map = map;
		this->size = size;
	}

	void Bullet::update(float deltaTime) {

		if (hitWall)
			return;

	
		float moveX = directionX * speed * deltaTime;
		float moveY = directionY * speed * deltaTime;

		float nextX = getPosition().x + moveX;
		float nextY = getPosition().y + moveY;

		
		if (map != nullptr && map->isWallArea(nextX, nextY, size, size)) {

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
