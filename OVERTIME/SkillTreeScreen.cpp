#include "pch.h"
#include "SkillTreeScreen.h"

SkillTreeScreen::SkillTreeScreen(sf::Font& font)
	: font(font)
{
}

void SkillTreeScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player)
{
	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);

	if (pressed && !mouseHeld)
	{
		mouseHeld = true;

		sf::Vector2i mousePixel = input.getMousePosition(window);

		sf::Vector2f mouse(
			static_cast<float>(mousePixel.x),
			static_cast<float>(mousePixel.y));

		auto& nodes = skillTree.GetNodes();

		for (size_t i = 0; i < nodes.size(); i++)
		{
			float dx = mouse.x - nodes[i].position.x;
			float dy = mouse.y - nodes[i].position.y;

			if (dx * dx + dy * dy <= 20.f * 20.f)
			{
				skillTree.Buy(static_cast<int>(i), player);
			}
		}
	}

	if (!pressed)
	{
		mouseHeld = false;
	}
}

void SkillTreeScreen::render(sf::RenderWindow& window, const EngineL::Player& player)
{
	const auto mousePixel = sf::Mouse::getPosition(window);
	const sf::Vector2f mouse = window.mapPixelToCoords(mousePixel);

	const SkillNode* hoveredNode = nullptr;

	const auto& nodes = skillTree.GetNodes();

	sf::Text text(font);

	text.setCharacterSize(26);
	text.setFillColor(sf::Color::White);

	text.setString("Souls: " + std::to_string(player.getSouls()));

	sf::FloatRect bounds = text.getLocalBounds();

	text.setPosition({
		static_cast<float>(window.getSize().x) - bounds.size.x - 20.f,
		20.f
		});

	window.draw(text);

	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	text.setString("Press ENTER\nTo Start Run");

	bounds = text.getLocalBounds();

	text.setPosition({
		static_cast<float>(window.getSize().x) - bounds.size.x - 20.f,
		static_cast<float>(window.getSize().y) - bounds.size.y - 30.f
		});

	window.draw(text);

	for (const auto& node : nodes)
	{
		for (int childIndex : node.children)
		{
			const auto& child = nodes[childIndex];

			sf::Vertex line[]
			{
				sf::Vertex(node.position, sf::Color(120, 120, 120)),
				sf::Vertex(child.position, sf::Color(120, 120, 120))
			};

			window.draw(line, 2, sf::PrimitiveType::Lines);
		}
	}

	for (const auto& node : nodes)
	{
		sf::CircleShape circle(20.f);

		circle.setOrigin({ 20.f, 20.f });
		circle.setPosition(node.position);

		if (node.level >= node.maxLevel)
		{
			circle.setFillColor(sf::Color(255, 215, 0));
		}
		else if (!node.unlocked)
		{
			circle.setFillColor(sf::Color(80, 80, 80));
		}
		else if (player.getSouls() < node.cost)
		{
			circle.setFillColor(sf::Color(200, 50, 50));
		}
		else
		{
			circle.setFillColor(sf::Color::Green);
		}

		float dx = mouse.x - node.position.x;
		float dy = mouse.y - node.position.y;

		if (dx * dx + dy * dy <= 20.f * 20.f)
		{
			hoveredNode = &node;

			circle.setScale({ 1.15f, 1.15f });
			circle.setOutlineThickness(3.f);
			circle.setOutlineColor(sf::Color::White);
		}

		window.draw(circle);
	}

	if (hoveredNode)
	{
		sf::RectangleShape box;

		box.setPosition(mouse + sf::Vector2f(25.f, 25.f));
		box.setSize({ 320.f, 180.f });

		box.setFillColor(sf::Color(25, 25, 25, 240));
		box.setOutlineThickness(2.f);
		box.setOutlineColor(sf::Color::White);

		window.draw(box);

		text.setCharacterSize(17);
		text.setFillColor(sf::Color::White);

		std::string status;

		if (hoveredNode->level >= hoveredNode->maxLevel)
		{
			status = "Max Level";
		}
		else if (hoveredNode->level > 0)
		{
			status = "Upgraded";
		}
		else if (hoveredNode->unlocked)
		{
			status = "Available";
		}
		else
		{
			status = "Locked";
		}

		text.setString(
			hoveredNode->name +
			"\n\n" +
			hoveredNode->description +
			"\n\nLevel: " +
			std::to_string(hoveredNode->level) +
			" / " +
			std::to_string(hoveredNode->maxLevel) +
			"\n\nCost: " +
			std::to_string(hoveredNode->cost) +
			" Souls" +
			"\nStatus: " +
			status);

		text.setPosition(mouse + sf::Vector2f(35.f, 35.f));

		window.draw(text);
	}
}