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
}

SkillTreeAction SkillTreeScreen::handleInput(
	EngineL::InputManager& input,
	const sf::RenderWindow& window,
	EngineL::Player& player,
	WeaponInventory& weaponInventory)
{
	// =========================
	// ZOOM
	// =========================

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


	// =========================
	// MOUSE
	// =========================

	bool pressed =
		input.isMouseButtonPressed(sf::Mouse::Button::Left);

	SkillTreeAction action = SkillTreeAction::None;


	// =========================
	// LEFT CLICK START
	// =========================

	if (pressed && !leftPressed)
	{
		leftPressed = true;
		dragging = false;

		dragStart = input.getMousePosition(window);
		lastMousePos = dragStart;
	}


	// =========================
	// DRAGGING
	// =========================

	if (pressed)
	{
		sf::Vector2i current =
			input.getMousePosition(window);

		float dx =
			static_cast<float>(current.x - dragStart.x);

		float dy =
			static_cast<float>(current.y - dragStart.y);

		// Start dragging after moving 5 pixels
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


	// =========================
	// LEFT CLICK RELEASE
	// =========================

	if (!pressed && leftPressed)
	{
		leftPressed = false;


		// =========================
		// ONLY PROCESS CLICK
		// IF WE DID NOT DRAG
		// =========================

		if (!dragging)
		{
			sf::Vector2i mousePixel =
				input.getMousePosition(window);

			// UI buttons use screen coordinates
			sf::Vector2f screenMouse(
				static_cast<float>(mousePixel.x),
				static_cast<float>(mousePixel.y));


			// =========================
			// PENDING CONFIRMATION
			// =========================

			if (pendingNodeIndex != -1)
			{
				if (kConfirmButtonBounds.contains(screenMouse))
				{
					skillTree.Buy(
						pendingNodeIndex,
						player,
						weaponInventory);

					pendingNodeIndex = -1;

					action = SkillTreeAction::RestartRun;
				}
				else if (kCancelButtonBounds.contains(screenMouse))
				{
					pendingNodeIndex = -1;
				}
			}


			// =========================
			// BACK BUTTON
			// =========================

			else if (
				duringRun &&
				kBackButtonBounds.contains(screenMouse))
			{
				action = SkillTreeAction::Back;
			}


			// =========================
			// SKILL NODE CLICK
			// =========================

			else
			{
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
						// Don't allow buying if
						// the node can't be bought.
						if (!canAfford(nodes[i], player))
							break;

						if (duringRun)
						{
							// During a run:
							// ask for confirmation.
							pendingNodeIndex =
								static_cast<int>(i);
						}
						else
						{
							// Outside a run:
							// buy immediately.
							skillTree.Buy(
								static_cast<int>(i),
								player,
								weaponInventory);
						}

						break;
					}
				}
			}
		}

		dragging = false;
	}


	return action;
}

void SkillTreeScreen::render(sf::RenderWindow& window, const EngineL::Player& player)
{
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

	if (!duringRun)
	{
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
				"\n\nCost: " +
				std::to_string(hoveredNode->cost) +
				" Souls";
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

	if (duringRun)
	{
		drawButton(window, kBackButtonBounds, isFrench ? "Retour" : "Back");
	}

	if (pendingNodeIndex != -1)
	{
		drawConfirmPopup(window);
	}
}

void SkillTreeScreen::drawConfirmPopup(sf::RenderWindow& window)
{
	bool isFrench = Language::current == LanguageOption::French;

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
		isFrench
		? "Debloquer cette competence maintenant\nva relancer la partie en cours.\n\nContinuer ?"
		: "Unlocking this skill now\nwill restart the current run.\n\nContinue ?");

	sf::FloatRect messageBounds = message.getLocalBounds();
	message.setPosition({
		window.getSize().x / 2.f - messageBounds.size.x / 2.f,
		window.getSize().y / 2.f - 130.f
		});
	window.draw(message);

	drawButton(window, kConfirmButtonBounds, isFrench ? "Confirmer" : "Confirm");
	drawButton(window, kCancelButtonBounds, isFrench ? "Annuler" : "Cancel");
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