#include "pch.h"
#include "Window.h"

namespace EngineL {

    Window::Window(unsigned int width, unsigned int height, const std::string& title)
        : window(sf::VideoMode({ width, height }), title)
    {
        window.setFramerateLimit(60);
    }

    bool Window::isOpen() const
    {
        return window.isOpen();
    }

    void Window::pollEvents()
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }

    void Window::clear()
    {
        window.clear(sf::Color::Black);
    }

    void Window::display()
    {
        window.display();
    }

    void Window::close()
    {
        window.close();
    }

    sf::RenderWindow& Window::getRenderWindow()
    {
        return window;
    }
}