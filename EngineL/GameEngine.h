#pragma once
#include "Window.h"
#include "InputManager.h"
#include "Renderer.h"

namespace EngineL {

	class GameEngine
	{
	public:
		GameEngine(unsigned int width, unsigned int height, const std::string& title);

		bool isRunning();
		float beginFrame();
		void endFrame();

		Window& getWindow();
		InputManager& getInputManager();
		Renderer& getRenderer();

	private:

		Window window;
		InputManager inputManager;
		Renderer renderer;
		sf::Clock clock;
	};
}