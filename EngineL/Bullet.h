#pragma once
#include "Entity.h"
#include "Map.h"

namespace EngineL {
	class Bullet : public Entity {

	public:

		Bullet(float x, float y, float directionX, float directionY, float damage,
			sf::Color color = sf::Color::Yellow, float width = 8.f, float height = 8.f, Map* map = nullptr);

		void update(float deltaTime) override;
		float getDamage() const;

	private:

		float directionX;
		float directionY;
		float speed = 600.f;
		float damage;
		Map* map;
	};
}