#pragma once
#include "Entity.h"
#include "Map.h"

namespace EngineL {
	class Bullet : public Entity {

	public:

		Bullet(float x, float y, float directionX, float directionY, float damage, Map* map,
			sf::Color color = sf::Color::Yellow, float size = 8.f);

		void update(float deltaTime) override;
		float getDamage() const;
		bool hasHitWall() const;

	private:

		float directionX;
		float directionY;

		float speed = 600.f;
		float damage;
		float size;
		bool hitWall = false;
		Map* map;
	};
}
