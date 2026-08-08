#pragma once
#include "Entity.h"

namespace EngineL
{
	class Bullet : public Entity {

	public:

		Bullet(float x, float y, float directionX, float directionY, float damage, sf::Color color = sf::Color::Yellow);

		void update(float deltaTime) override;

		float getDamage() const;

	private:

		float directionX;
		float directionY;
		float speed = 600.f;
		float damage;
	};
}