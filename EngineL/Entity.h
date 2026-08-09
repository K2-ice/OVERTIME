#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Stats.h"

namespace EngineL
{
	class Entity : public GameObject
	{
	public:
		Entity(float x, float y, float width, float height, sf::Color color);

		void update(float deltaTime) override;
		void render(Renderer& renderer) override;

		void setPosition(float x, float y);
		sf::Vector2f getPosition() const;
		void move(float offsetX, float offsetY);

		void takeDamage(int amount);
		bool isAlive() const;
		float getHealth() const;
		void setStats(const Stats& newStats);
		Stats& GetStats();
		const Stats& GetStats() const;
		sf::FloatRect getBounds() const;
	protected:
		sf::RectangleShape shape;
		Stats stats;
	};
}