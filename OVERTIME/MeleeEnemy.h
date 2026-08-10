#pragma once
#include "Enemy.h"

namespace EngineL {

	class MeleeEnemy : public Enemy {

	public:
		MeleeEnemy(float x, float y, Player* player, float difficulty, Map* map);
	};
}