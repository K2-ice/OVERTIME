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

	private:
		Player* player;
	};
}