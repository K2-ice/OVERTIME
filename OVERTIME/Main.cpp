#include <SFML/Graphics.hpp>

int main()
{
    // Création de la fenêtre : 800x600, titre "OVERTIME - Test SFML"
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "OVERTIME - Test SFML");
    window.setFramerateLimit(60);

    // Un simple cercle pour vérifier que le rendu fonctionne
    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({ 375.f, 275.f });

    while (window.isOpen())
    {
        // SFML 3 : pollEvent renvoie un std::optional<sf::Event>
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(shape);
        window.display();
    }

    return 0;
}