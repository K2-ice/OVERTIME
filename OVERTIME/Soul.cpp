#include "pch.h"
#include "Soul.h"

namespace EngineL
{
	Soul::Soul(float x, float y, int value) : Entity(x, y, 32.f, 32.f, sf::Color::Cyan) {

		this->value = value;

		setTexture("Assets/PNG (Transparent)/flare_01.png");
	}

	void Soul::update(float deltaTime) {}

	bool Soul::isCollected() const {
		return collected;
	}

	void Soul::collect() {
		collected = true;
	}

	int Soul::getValue() const {
		return value;
	}
}