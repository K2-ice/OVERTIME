#include "pch.h"
#include "SkillTreeScreen.h"

namespace
{
	const sf::FloatRect kBackButtonBounds({ 20.f, 20.f }, { 160.f, 50.f });
	const sf::FloatRect kConfirmButtonBounds({ 480.f, 430.f }, { 140.f, 60.f });
	const sf::FloatRect kCancelButtonBounds({ 660.f, 430.f }, { 140.f, 60.f });
}

SkillTreeScreen::SkillTreeScreen(sf::Font& font)
	: font(font)
{
}

void SkillTreeScreen::setDuringRun(bool duringRun)
{
	this->duringRun = duringRun;
	pendingNodeIndex = -1;
}

bool SkillTreeScreen::isDuringRun() const
{
	return duringRun;
}

bool SkillTreeScreen::canAfford(const SkillNode& node, const EngineL::Player& player) const
{
	return node.unlocked && node.level < node.maxLevel && player.getSouls() >= node.cost;
}

SkillTreeAction SkillTreeScreen::handleInput(EngineL::InputManager& input, const sf::RenderWindow& window, EngineL::Player& player)
{
	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);

	SkillTreeAction action = SkillTreeAction::None;

	if (pressed && !mouseHeld)
	{
		mouseHeld = true;

		sf::Vector2i mousePixel = input.getMousePosition(window);
		sf::Vector2f mouse(
			static_cast<float>(mousePixel.x),
			static_cast<float>(mousePixel.y));

		if (pendingNodeIndex != -1)
		{
			// --- Popup de confirmation active : on ignore tout sauf ses 2 boutons ---
			if (kConfirmButtonBounds.contains(mouse))
			{
				skillTree.Buy(pendingNodeIndex, player);
				pendingNodeIndex = -1;
				action = SkillTreeAction::RestartRun;
			}
			else if (kCancelButtonBounds.contains(mouse))
			{
				pendingNodeIndex = -1;
			}
		}
		else if (duringRun && kBackButtonBounds.contains(mouse))
		{
			action = SkillTreeAction::Back;
		}
		else
		{
			auto& nodes = skillTree.GetNodes();

			for (size_t i = 0; i < nodes.size(); i++)
			{
				float dx = mouse.x - nodes[i].position.x;
				float dy = mouse.y - nodes[i].position.y;

				if (dx * dx + dy * dy <= 20.f * 20.f)
				{
					if (!canAfford(nodes[i], player))
						continue;

					if (duringRun)
					{
						// On demande confirmation avant de debloquer pendant une run
						pendingNodeIndex = static_cast<int>(i);
					}
					else
					{
						// Entre deux runs : achat immediat, comme avant
						skillTree.Buy(static_cast<int>(i), player);
					}
				}
			}
		}
	}

	if (!pressed)
	{
		mouseHeld = false;
	}

	return action;
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

	if (!duringRun)
	{
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setString("Press ENTER\nTo Start Run");

		bounds = text.getLocalBounds();
		text.setPosition({
			static_cast<float>(window.getSize().x) - bounds.size.x - 20.f,
			static_cast<float>(window.getSize().y) - bounds.size.y - 30.f
			});
		window.draw(text);
	}

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
			status = "Max Level";
		else if (hoveredNode->level > 0)
			status = "Upgraded";
		else if (hoveredNode->unlocked)
			status = "Available";
		else
			status = "Locked";

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

	if (duringRun)
	{
		drawButton(window, kBackButtonBounds, "Retour");
	}

	if (pendingNodeIndex != -1)
	{
		drawConfirmPopup(window);
	}
}

void SkillTreeScreen::drawConfirmPopup(sf::RenderWindow& window)
{
	sf::RectangleShape overlay;
	overlay.setSize({ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) });
	overlay.setFillColor(sf::Color(0, 0, 0, 180));
	window.draw(overlay);

	sf::RectangleShape box;
	box.setSize({ 500.f, 220.f });
	box.setPosition({
		window.getSize().x / 2.f - 250.f,
		window.getSize().y / 2.f - 150.f
		});
	box.setFillColor(sf::Color(30, 30, 30));
	box.setOutlineThickness(2.f);
	box.setOutlineColor(sf::Color::White);
	window.draw(box);

	sf::Text message(font);
	message.setCharacterSize(20);
	message.setFillColor(sf::Color::White);
	message.setString(
		"Debloquer cette competence maintenant\n"
		"va relancer la partie en cours.\n\n"
		"Continuer ?");

	sf::FloatRect messageBounds = message.getLocalBounds();
	message.setPosition({
		window.getSize().x / 2.f - messageBounds.size.x / 2.f,
		window.getSize().y / 2.f - 130.f
		});
	window.draw(message);

	drawButton(window, kConfirmButtonBounds, "Confirmer");
	drawButton(window, kCancelButtonBounds, "Annuler");
}

void SkillTreeScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds, const std::string& label)
{
	sf::RectangleShape box;
	box.setPosition(bounds.position);
	box.setSize(bounds.size);
	box.setFillColor(sf::Color(50, 50, 50));
	box.setOutlineThickness(2.f);
	box.setOutlineColor(sf::Color::White);
	window.draw(box);

	sf::Text text(font);
	text.setCharacterSize(22);
	text.setFillColor(sf::Color::White);
	text.setString(label);

	sf::FloatRect textBounds = text.getLocalBounds();
	text.setPosition({
		bounds.position.x + bounds.size.x / 2.f - textBounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f - textBounds.size.y / 2.f - 5.f
		});
	window.draw(text);
}