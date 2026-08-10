#pragma once
#include <SFML/Graphics.hpp>

namespace EngineL {

	class Renderer {
	public:
		Renderer(sf::RenderWindow& window);

		void drawRectangle(const sf::RectangleShape& rectangle);
		void drawCircle(const sf::CircleShape& circle);
		void drawSprite(const sf::Sprite& sprite);
		void drawText(const sf::Text& text);

	private:

		sf::RenderWindow& window;
	};
}