#include "pch.h"
#include "Renderer.h"

namespace EngineL {

    Renderer::Renderer(sf::RenderWindow& window): window(window){}

    void Renderer::drawRectangle(const sf::RectangleShape& rectangle) {

        window.draw(rectangle);
    }

    void Renderer::drawCircle(const sf::CircleShape& circle) {
        window.draw(circle);
    }

    void Renderer::drawSprite(const sf::Sprite& sprite) {
        window.draw(sprite);
    }

    void Renderer::drawText(const sf::Text& text) {
        window.draw(text);
    }
}