#pragma once
#include "Entity.h"

namespace EngineL {

	class CollisionManager {

	public:

		static bool checkCollision(const Entity* a, const Entity* b);
	};
}