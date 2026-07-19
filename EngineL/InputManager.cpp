#include "pch.h"
#include "InputManager.h"

namespace EngineL
{
	void InputManager::update(const sf::RenderWindow& window)
	{
		for (int i = 0; i < KeyCount; i++)
		{
			previousKeys[i] = currentKeys[i];
		}

		for (int i = 0; i < KeyCount; i++)
		{
			currentKeys[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		}

		for (int i = 0; i < MouseButtonCount; i++)
		{
			currentMouseButtons[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
		}
	}

	bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
	{
		int index = static_cast<int>(key);
		return currentKeys[index];
	}

	bool InputManager::isKeyJustPressed(sf::Keyboard::Key key) const
	{
		int index = static_cast<int>(key);
		return currentKeys[index] && !previousKeys[index];
	}

	bool InputManager::isMouseButtonPressed(sf::Mouse::Button button) const
	{
		int index = static_cast<int>(button);
		return currentMouseButtons[index];
	}

	sf::Vector2i InputManager::getMousePosition(const sf::RenderWindow& window) const
	{
		return sf::Mouse::getPosition(window);
	}

	bool InputManager::isMovingUp() const
	{
		return isKeyPressed(sf::Keyboard::Key::Z);
	}

	bool InputManager::isMovingDown() const
	{
		return isKeyPressed(sf::Keyboard::Key::S);
	}

	bool InputManager::isMovingLeft() const
	{
		return isKeyPressed(sf::Keyboard::Key::Q);
	}

	bool InputManager::isMovingRight() const
	{
		return isKeyPressed(sf::Keyboard::Key::D);
	}

	bool InputManager::isPausePressed() const
	{
		return isKeyJustPressed(sf::Keyboard::Key::Escape);
	}

	bool InputManager::isStartPressed() const
	{
		return isKeyJustPressed(sf::Keyboard::Key::Enter);
	}
}