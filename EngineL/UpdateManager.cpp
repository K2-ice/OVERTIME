#include "pch.h"
#include "UpdateManager.h"

namespace EngineL
{
	void UpdateManager::add(GameObject* object)
	{
		objects.push_back(object);
	}

	void UpdateManager::updateAll(float deltaTime)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->update(deltaTime);
		}
	}

	void UpdateManager::remove(GameObject* object)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == object)
			{
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}
}