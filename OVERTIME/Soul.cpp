#include "pch.h"
#include "Soul.h"

namespace EngineL
{
	Soul::Soul(float x, float y, int value)
		: Entity(x, y, 16.f, 16.f, sf::Color::Cyan)
	{
		this->value = value;
	}

	void Soul::update(float deltaTime)
	{
	}

	bool Soul::isCollected() const
	{
		return collected;
	}

	void Soul::collect()
	{
		collected = true;
	}

	int Soul::getValue() const
	{
		return value;
	}
}