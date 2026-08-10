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
	treeView.setSize({ 1280.f, 720.f });
	treeView.setCenter({ 300.f, 100.f });
	difficultyButtonBounds[0] = {
		{ 40.f, 620.f },
		{ 50.f, 50.f }
	};

	difficultyButtonBounds[1] = {
		{ 100.f, 620.f },
		{ 50.f, 50.f }
	};

	difficultyButtonBounds[2] = {
		{ 160.f, 620.f },
		{ 50.f, 50.f }
	};

	difficultyButtonBounds[3] = {
		{ 220.f, 620.f },
		{ 50.f, 50.f }
	};
}

void SkillTreeScreen::setDuringRun(bool duringRun)
{
	this->duringRun = duringRun;
	pendingNodeIndex = -1;
	pendingDifficulty = -1;

	if (duringRun)
		WarningShown = false;
}

bool SkillTreeScreen::isDuringRun() const
{
	return duringRun;
}

bool SkillTreeScreen::canAfford(
	const SkillNode& node,
	const EngineL::Player& player) const
{
	return node.unlocked
		&& node.level < node.maxLevel
		&& player.getSouls() >= node.cost;
}

bool SkillTreeScreen::isDifficultyUnlocked(
	int difficulty, const EngineL::Player& player)
{
	const auto& nodes = skillTree.GetNodes();

	for (const auto& node : nodes)
	{
		if (player.GetStats().maxDifficulty >= difficulty)
		{
			return node.unlocked;
		}
	}

	return difficulty == 1;
}

SkillTreeAction SkillTreeScreen::handleInput(
	EngineL::InputManager& input,
	const sf::RenderWindow& window,
	EngineL::Player& player,
	WeaponInventory& weaponInventory)
{
	float wheel = input.getMouseWheelDelta();

	if (wheel > 0.f)
		zoom *= 0.9f;
	else if (wheel < 0.f)
		zoom *= 1.1f;

	zoom = std::clamp(zoom, minZoom, maxZoom);

	treeView.setSize({
		1280.f * zoom,
		720.f * zoom
		});

	bool pressed =
		input.isMouseButtonPressed(sf::Mouse::Button::Left);

	SkillTreeAction action = SkillTreeAction::None;

	if (pressed && !leftPressed)
	{
		leftPressed = true;
		dragging = false;

		dragStart = input.getMousePosition(window);
		lastMousePos = dragStart;
	}

	if (pressed)
	{
		sf::Vector2i current =
			input.getMousePosition(window);

		float dx =
			static_cast<float>(current.x - dragStart.x);

		float dy =
			static_cast<float>(current.y - dragStart.y);

		if (!dragging &&
			dx * dx + dy * dy > 25.f)
		{
			dragging = true;
		}

		if (dragging)
		{
			sf::Vector2f before =
				window.mapPixelToCoords(
					lastMousePos,
					treeView);

			sf::Vector2f after =
				window.mapPixelToCoords(
					current,
					treeView);

			treeView.move(before - after);

			lastMousePos = current;
		}
	}

	if (!pressed && leftPressed)
	{
		leftPressed = false;

		if (!dragging)
		{
			sf::Vector2i mousePixel =
				input.getMousePosition(window);

			sf::Vector2f screenMouse(
				static_cast<float>(mousePixel.x),
				static_cast<float>(mousePixel.y));

			if (pendingDifficulty != -1 && pendingDifficulty != -2)
			{
				if (kConfirmButtonBounds.contains(screenMouse))
				{
					player.GetStats().difficulty =
						pendingDifficulty;

					WarningShown = true;
					duringRun = false;
					pendingDifficulty = -1;

					return SkillTreeAction::None;
				}

				if (kCancelButtonBounds.contains(screenMouse))
				{
					WarningShown = false;
					pendingDifficulty = -1;
				}

				dragging = false;
				return SkillTreeAction::None;
			}

			if (pendingNodeIndex != -1 && pendingNodeIndex != -2)
			{
				if (kConfirmButtonBounds.contains(screenMouse))
				{
					skillTree.Buy(
						pendingNodeIndex,
						player,
						weaponInventory);
					WarningShown = true;
					duringRun = false;
					pendingNodeIndex = -1;

					return SkillTreeAction::None;
				}

				if (kCancelButtonBounds.contains(screenMouse))
				{
					WarningShown = false;
					pendingNodeIndex = -1;
				}

				dragging = false;
				return SkillTreeAction::None;
			}

			if (duringRun &&
				kBackButtonBounds.contains(screenMouse))
			{
				dragging = false;
				return SkillTreeAction::Back;
			}

			for (int i = 0; i < 4; i++)
			{
				if (!difficultyButtonBounds[i].contains(screenMouse))
					continue;

				int newDifficulty = i + 1;

				if (!isDifficultyUnlocked(newDifficulty, player))
				{
					dragging = false;
					return SkillTreeAction::None;
				}

				if (newDifficulty == player.GetStats().difficulty)
				{
					dragging = false;
					return SkillTreeAction::None;
				}

				if (duringRun)
				{
					pendingDifficulty = newDifficulty;
				}
				else
				{
					player.GetStats().difficulty = newDifficulty;
				}

				dragging = false;
				return SkillTreeAction::None;
			}

			sf::Vector2f mouse =
				window.mapPixelToCoords(
					mousePixel,
					treeView);

			auto& nodes =
				skillTree.GetNodes();

			for (size_t i = 0;
				i < nodes.size();
				i++)
			{
				float dx =
					mouse.x - nodes[i].position.x;

				float dy =
					mouse.y - nodes[i].position.y;

				if (dx * dx + dy * dy <=
					20.f * 20.f)
				{
					if (!canAfford(
						nodes[i],
						player))
					{
						break;
					}

					if (duringRun)
					{
						pendingNodeIndex = static_cast<int>(i);
					}
					else
					{
						skillTree.Buy(
							static_cast<int>(i),
							player,
							weaponInventory);
					}

					break;
				}
			}
		}

		dragging = false;
	}

	return action;
}
void SkillTreeScreen::render(sf::RenderWindow& window, const EngineL::Player& player)
{
	window.setView(window.getDefaultView());

	sf::RectangleShape background;
	background.setSize({
		static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y)
		});

	background.setFillColor(sf::Color(218, 190, 140));

	window.draw(background);

	bool isFrench = Language::current == LanguageOption::French;

	const auto mousePixel = sf::Mouse::getPosition(window);
	const sf::Vector2f mouse =
		window.mapPixelToCoords(mousePixel, treeView);

	const SkillNode* hoveredNode = nullptr;

	const auto& nodes = skillTree.GetNodes();

	sf::Text text(font);

	text.setCharacterSize(26);
	text.setFillColor(sf::Color::White);
	text.setString((isFrench ? "Ames: " : "Souls: ") + std::to_string(player.getSouls()));

	sf::FloatRect bounds = text.getLocalBounds();
	text.setPosition({
		static_cast<float>(window.getSize().x) - bounds.size.x - 20.f,
		20.f
		});
	window.draw(text);
	if (!duringRun){

		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);
		text.setString(isFrench ? "Appuyez sur ENTREE\npour commencer" : "Press ENTER\nTo Start Run");

		bounds = text.getLocalBounds();
		text.setPosition({
			static_cast<float>(window.getSize().x) - bounds.size.x - 20.f,
			static_cast<float>(window.getSize().y) - bounds.size.y - 30.f
			});
		window.draw(text);
	}

	if (pendingDifficulty != -1 && pendingDifficulty != -2)
	{
		sf::RectangleShape box;
		box.setSize({ 420.f, 180.f });
		box.setPosition({
			static_cast<float>(window.getSize().x) / 2.f - 210.f,
			static_cast<float>(window.getSize().y) / 2.f - 90.f
			});

		box.setFillColor(sf::Color(25, 25, 25, 245));
		box.setOutlineThickness(3.f);
		box.setOutlineColor(sf::Color::White);

		window.draw(box);

		sf::Text warning(font);
		warning.setCharacterSize(22);
		warning.setFillColor(sf::Color::White);

		std::string message;

		if (isFrench)
		{
			message =
				"Changer la difficulte\n"
				"arretera le run.\n\n"
				"Continuer ?";
		}
		else
		{
			message =
				"Changing difficulty\n"
				"will stop the run.\n\n"
				"Continue?";
		}

		warning.setString(message);

		sf::FloatRect bounds =
			warning.getLocalBounds();

		warning.setPosition({
			static_cast<float>(window.getSize().x) / 2.f
				- bounds.size.x / 2.f,
			static_cast<float>(window.getSize().y) / 2.f
				- bounds.size.y / 2.f
			});

		window.draw(warning);

		drawButton(
			window,
			kConfirmButtonBounds,
			isFrench ? "Confirmer" : "Confirm");

		drawButton(
			window,
			kCancelButtonBounds,
			isFrench ? "Annuler" : "Cancel");
	}

	window.setView(treeView);

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
		bool isDifficulty = node.type == SkillType::Difficulty;
		bool isWeaponSlot = node.type == SkillType::WeaponSlot;
		bool isShotgun = node.type == SkillType::UnlockShotgun;
		bool isSubmachinegun = node.type == SkillType::UnlockSubmachinegun;

		sf::Color fillColor;

		if (node.level >= node.maxLevel)
		{
			fillColor = sf::Color(255, 215, 0);
		}
		else if (!node.unlocked)
		{
			fillColor = sf::Color(80, 80, 80);
		}
		else if (player.getSouls() < node.cost)
		{
			fillColor = sf::Color(200, 50, 50);
		}
		else
		{
			fillColor = sf::Color::Green;
		}

		float dx = mouse.x - node.position.x;
		float dy = mouse.y - node.position.y;

		bool hovered = dx * dx + dy * dy <= 20.f * 20.f;

		if (isDifficulty)
		{
			sf::RectangleShape diamond({ 28.f, 28.f });

			diamond.setOrigin({ 14.f, 14.f });
			diamond.setPosition(node.position);
			diamond.setRotation(sf::degrees(45.f));
			diamond.setFillColor(fillColor);

			if (hovered)
			{
				hoveredNode = &node;

				diamond.setScale({ 1.15f, 1.15f });
				diamond.setOutlineThickness(3.f);
				diamond.setOutlineColor(sf::Color::White);
			}

			window.draw(diamond);
		}
		else if (isWeaponSlot || isShotgun || isSubmachinegun)
		{
			sf::RectangleShape square({ 32.f, 32.f });

			square.setOrigin({ 16.f, 16.f });
			square.setPosition(node.position);
			square.setFillColor(fillColor);

			if (hovered)
			{
				hoveredNode = &node;

				square.setScale({ 1.15f, 1.15f });
				square.setOutlineThickness(3.f);
				square.setOutlineColor(sf::Color::White);
			}

			window.draw(square);
		}
		else
		{
			sf::CircleShape circle(20.f);

			circle.setOrigin({ 20.f, 20.f });
			circle.setPosition(node.position);
			circle.setFillColor(fillColor);

			if (hovered)
			{
				hoveredNode = &node;

				circle.setScale({ 1.15f, 1.15f });
				circle.setOutlineThickness(3.f);
				circle.setOutlineColor(sf::Color::White);
			}

			window.draw(circle);
		}
	}

	if (hoveredNode)
	{
		sf::RectangleShape box;
		box.setPosition(mouse + sf::Vector2f(25.f, 25.f));
		box.setSize({ 330.f, 200.f });

		box.setFillColor(sf::Color(25, 25, 25, 240));
		box.setOutlineThickness(2.f);
		box.setOutlineColor(sf::Color::White);
		window.draw(box);

		text.setCharacterSize(17);
		text.setFillColor(sf::Color::White);

		std::string status;

		if (hoveredNode->level >= hoveredNode->maxLevel)
			status = isFrench ? "Niveau max" : "Max Level";
		else if (hoveredNode->level > 0)
			status = isFrench ? "Ameliore" : "Upgraded";
		else if (hoveredNode->unlocked)
			status = isFrench ? "Disponible" : "Available";
		else
			status = isFrench ? "Verrouille" : "Locked";

		std::string costLabel = isFrench ? " Ames" : " Souls";
		std::string levelLabel = isFrench ? "Niveau: " : "Level: ";
		std::string costPrefix = isFrench ? "Cout: " : "Cost: ";
		std::string statusPrefix = isFrench ? "\nEtat: " : "\nStatus: ";

		std::string info =
			hoveredNode->name +
			"\n\n" +
			hoveredNode->description +
			"\n\n" + levelLabel +
			std::to_string(hoveredNode->level) +
			" / " +
			std::to_string(hoveredNode->maxLevel);

		if (hoveredNode->level < hoveredNode->maxLevel)
		{
			info +=
				"\n\n" + costPrefix + ": " +
				std::to_string(hoveredNode->cost) +
				costLabel;
		}
		else
		{
			info += "\n\n";
		}

		info +=
			"\nStatus: " +
			status;

		text.setString(info);

		text.setPosition(mouse + sf::Vector2f(35.f, 35.f));
		window.draw(text);
	}
	window.setView(window.getDefaultView());

	if (duringRun && !WarningShown)
	{
		drawButton(window, kBackButtonBounds, isFrench ? "Retour" : "Back");
	}

	drawDifficultySelector(window, player);

	if (pendingNodeIndex != -1 && pendingNodeIndex != -2)
	{
		drawConfirmPopup(window);
	}
}

void SkillTreeScreen::drawDifficultySelector(
	sf::RenderWindow& window,
	const EngineL::Player& player)
{
	bool isFrench =
		Language::current == LanguageOption::French;

	sf::Text title(font);
	title.setCharacterSize(20);
	title.setFillColor(sf::Color::White);

	title.setString(
		isFrench ? "Difficulte" : "Difficulty");

	title.setPosition({ 40.f, 585.f });

	window.draw(title);

	for (int i = 0; i < 4; i++)
	{
		int difficulty = i + 1;

		bool unlocked =
			isDifficultyUnlocked(difficulty, player);

		sf::RectangleShape button({
			50.f,
			50.f
			});

		button.setPosition({
			40.f + i * 60.f,
			620.f
			});

		if (!unlocked)
		{
			button.setFillColor(
				sf::Color(40, 40, 40));
			button.setOutlineColor(
				sf::Color(70, 70, 70));
		}
		else if (player.GetStats().difficulty == difficulty)
		{
			button.setFillColor(
				sf::Color(255, 215, 0));
			button.setOutlineColor(
				sf::Color::White);
		}
		else
		{
			button.setFillColor(
				sf::Color(80, 80, 80));
			button.setOutlineColor(
				sf::Color::White);
		}

		button.setOutlineThickness(2.f);

		window.draw(button);

		sf::Text number(font);
		number.setCharacterSize(24);

		if (unlocked)
		{
			number.setFillColor(sf::Color::White);
		}
		else
		{
			number.setFillColor(
				sf::Color(100, 100, 100));
		}

		number.setString(
			std::to_string(difficulty));

		sf::FloatRect bounds =
			number.getLocalBounds();

		number.setPosition({
			40.f + i * 60.f +
				25.f - bounds.size.x / 2.f,
			620.f +
				25.f - bounds.size.y / 2.f -
				5.f
			});

		window.draw(number);

		difficultyButtonBounds[i] =
			button.getGlobalBounds();
	}
}

void SkillTreeScreen::drawConfirmPopup(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());

	bool isFrench =
		Language::current == LanguageOption::French;

	sf::RectangleShape overlay;
	overlay.setSize({
		static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y)
		});
	overlay.setFillColor(sf::Color(218, 190, 140));
	window.draw(overlay);

	sf::RectangleShape box;
	box.setSize({ 500.f, 220.f });
	box.setPosition({
		static_cast<float>(window.getSize().x) / 2.f - 250.f,
		static_cast<float>(window.getSize().y) / 2.f - 150.f
		});
	box.setFillColor(sf::Color(30, 30, 30));
	box.setOutlineThickness(2.f);
	box.setOutlineColor(sf::Color::White);

	window.draw(box);

	sf::Text message(font);
	message.setCharacterSize(20);
	message.setFillColor(sf::Color::White);

	message.setString(
		isFrench
		? "Debloquer cette competence maintenant\n"
		"va relancer la partie en cours.\n\n"
		"Continuer ?"
		: "Unlocking this skill now\n"
		"will restart the current run.\n\n"
		"Continue ?");

	sf::FloatRect messageBounds =
		message.getLocalBounds();

	message.setPosition({
		static_cast<float>(window.getSize().x) / 2.f
			- messageBounds.size.x / 2.f,
		static_cast<float>(window.getSize().y) / 2.f
			- 130.f
		});

	window.draw(message);

	drawButton(
		window,
		kConfirmButtonBounds,
		isFrench ? "Confirmer" : "Confirm");

	drawButton(
		window,
		kCancelButtonBounds,
		isFrench ? "Annuler" : "Cancel");
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
	window.setView(window.getDefaultView());
}