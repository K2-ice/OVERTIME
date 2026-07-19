#include "pch.h"
#include "Entity.h"

namespace EngineL
{
	Entity::Entity(float x, float y, float width, float height, sf::Color color)
	{
		shape.setSize(sf::Vector2f(width, height));
		shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(color);
	}

	void Entity::update(float deltaTime)
	{
	}

	void Entity::render(Renderer& renderer)
	{
		renderer.drawRectangle(shape);
	}

	void Entity::setPosition(float x, float y)
	{
		shape.setPosition(sf::Vector2f(x, y));
	}

	sf::Vector2f Entity::getPosition() const
	{
		return shape.getPosition();
	}

	void Entity::move(float offsetX, float offsetY)
	{
		shape.move(sf::Vector2f(offsetX, offsetY));
	}

	void Entity::takeDamage(int amount)
	{
		stats.health -= amount;

		if (stats.health < 0)
			stats.health = 0;
	}

	bool Entity::isAlive() const
	{
		return stats.health > 0;
	}

	float Entity::getHealth() const
	{
		return stats.health;
	}

	void Entity::setStats(const Stats& newStats)
	{
		stats = newStats;
	}

	Stats& Entity::GetStats()
	{
		return stats;
	}

	const Stats& Entity::GetStats() const
	{
		return stats;
	}

	sf::FloatRect Entity::getBounds() const
	{
		return shape.getGlobalBounds();
	}
}