#include "pch.h"
#include "RenderManager.h"

namespace EngineL
{
	void RenderManager::add(GameObject* object) {
		objects.push_back(object);
	}

	void RenderManager::renderAll(Renderer& renderer) {

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->render(renderer);
		}
	}

	void RenderManager::remove(GameObject* object) {

		for (int i = 0; i < objects.size(); i++) {
			if (objects[i] == object) {
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}
}