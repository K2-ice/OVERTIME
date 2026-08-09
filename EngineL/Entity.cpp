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
		if (useSprite && sprite.has_value())
		{
			renderer.drawSprite(sprite.value());
		}
		else
		{
			renderer.drawRectangle(shape);
		}
	}
	void Entity::setPosition(float x, float y)
	{
		shape.setPosition(sf::Vector2f(x, y));
		if (sprite.has_value())
			sprite->setPosition(sf::Vector2f(x, y));
	}
	sf::Vector2f Entity::getPosition() const
	{
		return shape.getPosition();
	}
	void Entity::move(float offsetX, float offsetY)
	{
		shape.move(sf::Vector2f(offsetX, offsetY));
		if (sprite.has_value())
			sprite->move(sf::Vector2f(offsetX, offsetY));
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
		if (useSprite && sprite.has_value())
			return sprite->getGlobalBounds();
		return shape.getGlobalBounds();
	}

	bool Entity::setTexture(const std::string& filePath)
	{
		std::shared_ptr<sf::Texture> newTexture = std::make_shared<sf::Texture>();

		if (!newTexture->loadFromFile(filePath))
			return false;

		texture = newTexture;
		sprite.emplace(*texture);

		sf::Vector2f targetSize = shape.getSize();
		sf::Vector2u textureSize = texture->getSize();

		if (textureSize.x > 0 && textureSize.y > 0)
		{
			sprite->setScale(sf::Vector2f(
				targetSize.x / static_cast<float>(textureSize.x),
				targetSize.y / static_cast<float>(textureSize.y)));
		}

		sprite->setPosition(shape.getPosition());
		useSprite = true;

		return true;
	}
}