#include "pch.h"
#include "CollisionManager.h"

namespace EngineL
{
	bool CollisionManager::checkCollision(const Entity* a, const Entity* b)
	{
		if (a->getBounds().findIntersection(b->getBounds()))
			return true;

		return false;
	}
}