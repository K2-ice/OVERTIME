#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace EngineL {

	class Window {

	public:

		Window(unsigned int width, unsigned int height, const std::string& title);

		bool isOpen() const;
		void pollEvents();
		void clear();
		void display();
		void close();

		sf::RenderWindow& getRenderWindow();

	private:
		sf::RenderWindow window;
	};
}