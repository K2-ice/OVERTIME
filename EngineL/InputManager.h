#pragma once
#include <SFML/Graphics.hpp>

namespace EngineL {

	enum class KeyLayout {
		AZERTY,
		QWERTY
	};

	class InputManager {

	public:

		void update(const sf::RenderWindow& window);
		bool isKeyPressed(sf::Keyboard::Key key) const;
		bool isKeyJustPressed(sf::Keyboard::Key key) const;

		bool isMouseButtonPressed(sf::Mouse::Button button) const;
		sf::Vector2i getMousePosition(const sf::RenderWindow& window) const;

		bool isMovingUp() const;
		bool isMovingDown() const;
		bool isMovingLeft() const;
		bool isMovingRight() const;
		bool isPausePressed() const;
		bool isStartPressed() const;
		bool isReloadPressed() const;
		void handleEvent(const sf::Event& event);

		void setKeyLayout(KeyLayout layout);
		KeyLayout getKeyLayout() const;
		void toggleKeyLayout();

		float getMouseWheelDelta() const;
		void beginFrame();

	private:

		static const int KeyCount = 101;
		static const int MouseButtonCount = 5;

		bool currentKeys[KeyCount];
		bool previousKeys[KeyCount];

		bool currentMouseButtons[MouseButtonCount];
		float mouseWheelDelta = 0.f;

		KeyLayout keyLayout = KeyLayout::AZERTY;
	};
}