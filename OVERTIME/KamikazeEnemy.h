#pragma once
#include "Enemy.h"

namespace EngineL
{
	class KamikazeEnemy : public Enemy
	{
	public:
		KamikazeEnemy(float x, float y, Player* player, float difficulty, Map* map);

		bool explodesOnContact() const override;

	};
}