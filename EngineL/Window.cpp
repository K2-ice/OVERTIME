#include "pch.h"
#include "Window.h"
#include "pch.h"
#include "Window.h"

namespace Engine
{
    Window::Window(unsigned int width, unsigned int height, const std::string& title)
        : m_window(sf::VideoMode({ width, height }), title)
    {
        m_window.setFramerateLimit(60);
    }

    bool Window::isOpen() const
    {
        return m_window.isOpen();
    }

    void Window::pollEvents()
    {
        while (const std::optional event = m_window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                m_window.close();
        }
    }

    void Window::clear()
    {
        m_window.clear(sf::Color::Black);
    }

    void Window::display()
    {
        m_window.display();
    }

    void Window::close()
    {
        m_window.close();
    }

    sf::RenderWindow& Window::getRenderWindow()
    {
        return m_window;
    }
}