#pragma once

#include "Entity.h"

namespace EngineL
{
	class Soul : public Entity
	{
	public:
		Soul(float x, float y, int value = 1);

		void update(float deltaTime) override;

		bool isCollected() const;
		void collect();

		int getValue() const;

	private:
		int value;
		bool collected = false;
	};
}