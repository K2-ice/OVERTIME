// GameEngine.cpp
#include "pch.h"
#include "GameEngine.h"

namespace EngineL
{
    GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& title)
        : window(width, height, title)
        , renderer(window.getRenderWindow())
    {
    }

    bool GameEngine::isRunning()
    {
        return window.isOpen();
    }

    float GameEngine::beginFrame()
    {
        float deltaTime = clock.restart().asSeconds();

        window.pollEvents();
        inputManager.update(window.getRenderWindow());
        window.clear();

        return deltaTime;
    }

    void GameEngine::endFrame()
    {
        window.display();
    }

    Window& GameEngine::getWindow()
    {
        return window;
    }

    InputManager& GameEngine::getInputManager()
    {
        return inputManager;
    }

    Renderer& GameEngine::getRenderer()
    {
        return renderer;
    }
}