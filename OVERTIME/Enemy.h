#pragma once
#include "Entity.h"
#include "Player.h"

namespace EngineL
{
	class Enemy : public Entity
	{
	public:
		Enemy(float x, float y, Player* player);

		void update(float deltaTime) override;
		void render(Renderer& renderer) override;

		void takeDamage(int amount);

		virtual bool explodesOnContact() const;

	protected:
		void moveTowardPlayer(float deltaTime, float speedMultiplier = 1.f);

		Player* player;

	private:
		float healthBarTimer = 0.f;
		float healthBarDuration = 2.f;
	};
}