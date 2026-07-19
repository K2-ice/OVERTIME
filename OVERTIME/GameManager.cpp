#include "pch.h"
#include "GameManager.h"

#include <cmath>
#include <cstdlib>

GameManager::GameManager()
	: engine(1280, 720, "OVERTIME")
	, player(400.f, 300.f, &engine.getInputManager())
{
	updateManager.add(&player);
	renderManager.add(&player);

	if (!font.openFromFile("Assets/arial.ttf"))
		throw std::runtime_error("Failed to load font.");
	player.addSouls(50);
}

GameManager::~GameManager()
{
	for (auto bullet : bullets)
	{
		updateManager.remove(bullet);
		renderManager.remove(bullet);
		delete bullet;
	}

	for (auto enemy : enemies)
	{
		updateManager.remove(enemy);
		renderManager.remove(enemy);
		delete enemy;
	}

	for (auto soul : souls)
	{
		updateManager.remove(soul);
		renderManager.remove(soul);
		delete soul;
	}
}

void GameManager::run()
{
	while (engine.isRunning())
	{
		float deltaTime = engine.beginFrame();

		sf::Vector2i mouse =
			engine.getInputManager().getMousePosition(
				engine.getWindow().getRenderWindow());

		if (showSkillTree)
		{
			handleSkillTree();

			if (engine.getInputManager().isStartPressed())
			{
				startNewRun();
			}

			renderSkillTree();

			engine.endFrame();

			continue;
		}

		runTime += deltaTime;

		if (runTime >= maxRunTime)
		{
			showSkillTree = true;
		}

		player.aimAt(
			static_cast<float>(mouse.x),
			static_cast<float>(mouse.y));

		spawnTimer += deltaTime;

		if (spawnTimer >= spawnDelay)
		{
			spawnTimer = 0.f;
			spawnEnemy();
		}

		handleShooting();

		cleanupBullets();

		renderSkillTree();

		updateManager.updateAll(deltaTime);

		checkBulletEnemyCollisions();

		checkEnemyPlayerCollisions();

		cleanupEnemies();

		collectSouls();

		cleanupBullets();

		renderManager.renderAll(engine.getRenderer());

		renderSkillTree();

		engine.endFrame();
	}
}

void GameManager::handleShooting()
{
	if (player.wantsToShoot())
	{
		float x = player.getPosition().x;
		float y = player.getPosition().y;
		float dirX = player.getShootDirectionX();
		float dirY = player.getShootDirectionY();

		EngineL::Bullet* bullet = new EngineL::Bullet(x, y, dirX, dirY);

		bullets.push_back(bullet);
		updateManager.add(bullet);
		renderManager.add(bullet);

		player.resetShootCooldown();
	}
}

void GameManager::spawnEnemy()
{
	sf::Vector2f playerPos = player.getPosition();

	float angle =
		static_cast<float>(rand()) /
		static_cast<float>(RAND_MAX) *
		6.2831853f;

	float x = playerPos.x + std::cos(angle) * 500.f;
	float y = playerPos.y + std::sin(angle) * 500.f;

	EngineL::Enemy* enemy = new EngineL::Enemy(x, y, &player);

	enemies.push_back(enemy);

	updateManager.add(enemy);
	renderManager.add(enemy);
}


void GameManager::cleanupBullets()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		EngineL::Bullet* bullet = bullets[i];

		float x = bullet->getPosition().x;
		float y = bullet->getPosition().y;

		bool outOfScreen = x < 0.f || x > 1280.f || y < 0.f || y > 720.f;

		if (outOfScreen)
		{
			updateManager.remove(bullet);
			renderManager.remove(bullet);

			delete bullet;
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}
void GameManager::checkBulletEnemyCollisions()
{
	for (int b = 0; b < bullets.size(); b++)
	{
		EngineL::Bullet* bullet = bullets[b];

		bool bulletDestroyed = false;

		for (int e = 0; e < enemies.size(); e++)
		{
			EngineL::Enemy* enemy = enemies[e];

			if (bullet->getBounds().findIntersection(enemy->getBounds()))
			{
				enemy->takeDamage(
					static_cast<int>(player.GetStats().damage));

				updateManager.remove(bullet);
				renderManager.remove(bullet);

				delete bullet;

				bullets.erase(bullets.begin() + b);

				b--;

				bulletDestroyed = true;

				break;
			}
		}

		if (bulletDestroyed)
			continue;
	}
}

void GameManager::cleanupEnemies()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		EngineL::Enemy* enemy = enemies[i];

		if (!enemy->isAlive())
		{
			EngineL::Soul* soul =
				new EngineL::Soul(
					enemy->getPosition().x,
					enemy->getPosition().y);

			souls.push_back(soul);

			updateManager.add(soul);
			renderManager.add(soul);

			updateManager.remove(enemy);
			renderManager.remove(enemy);

			delete enemy;

			enemies.erase(enemies.begin() + i);

			i--;
		}
	}
}

void GameManager::collectSouls()
{
	for (int i = 0; i < souls.size(); i++)
	{
		EngineL::Soul* soul = souls[i];

		if (player.getBounds().findIntersection(soul->getBounds()))
		{
			player.addSouls(soul->getValue());

			updateManager.remove(soul);
			renderManager.remove(soul);

			delete soul;

			souls.erase(souls.begin() + i);

			i--;
		}
	}
}

void GameManager::checkEnemyPlayerCollisions()
{
	for (EngineL::Enemy* enemy : enemies)
	{
		if (enemy->getBounds().findIntersection(player.getBounds()))
		{
			if (player.canTakeDamage())
			{
				player.takeDamage(
					static_cast<int>(enemy->GetStats().damage));

				player.resetDamageCooldown();
			}
		}
	}
}
void GameManager::startNewRun()
{
	showSkillTree = false;
	runTime = 0.f;
	spawnTimer = 0.f;

	// Reset player
	player.setPosition(400.f, 300.f);

	Stats& stats = player.GetStats();
	stats.health = stats.maxHealth;

	// Remove bullets
	for (auto bullet : bullets)
	{
		updateManager.remove(bullet);
		renderManager.remove(bullet);
		delete bullet;
	}
	bullets.clear();

	// Remove enemies
	for (auto enemy : enemies)
	{
		updateManager.remove(enemy);
		renderManager.remove(enemy);
		delete enemy;
	}
	enemies.clear();

	// Remove souls
	for (auto soul : souls)
	{
		updateManager.remove(soul);
		renderManager.remove(soul);
		delete soul;
	}
	souls.clear();
}

void GameManager::handleSkillTree()
{
	auto& input = engine.getInputManager();
	bool pressed = input.isMouseButtonPressed(sf::Mouse::Button::Left);

	if (pressed && !mouseHeld)
	{
		mouseHeld = true;

		sf::Vector2i mousePixel =
			input.getMousePosition(engine.getWindow().getRenderWindow());

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
				skillTree.Buy(i, player);
			}
		}
	}

	if (!pressed)
	{
		mouseHeld = false;
	}
}
std::string GameManager::FormatTime(float seconds)
{
	int totalSeconds = static_cast<int>(seconds);

	int minutes = totalSeconds / 60;
	int secs = totalSeconds % 60;

	std::ostringstream stream;

	stream << std::setfill('0')
		<< std::setw(2) << minutes
		<< ":"
		<< std::setw(2) << secs;

	return stream.str();
}

void GameManager::renderSkillTree()
{

	if (showSkillTree)
	{
		const auto mousePixel = sf::Mouse::getPosition(engine.getWindow().getRenderWindow());
		const sf::Vector2f mouse = engine.getWindow().getRenderWindow().mapPixelToCoords(mousePixel);

		const SkillNode* hoveredNode = nullptr;

		const auto& nodes = skillTree.GetNodes();

		sf::Text text(font);

		text.setCharacterSize(26);
		text.setFillColor(sf::Color::White);

		text.setString("Souls: " + std::to_string(player.getSouls()));

		sf::FloatRect bounds = text.getLocalBounds();

		text.setPosition({
			static_cast<float>(engine.getWindow().getRenderWindow().getSize().x) - bounds.size.x - 20.f,
			20.f
			});

		engine.getWindow().getRenderWindow().draw(text);

		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		text.setString("Press ENTER\nTo Start Run");

		bounds = text.getLocalBounds();

		text.setPosition({
			static_cast<float>(engine.getWindow().getRenderWindow().getSize().x) - bounds.size.x - 20.f,
			static_cast<float>(engine.getWindow().getRenderWindow().getSize().y) - bounds.size.y - 30.f
			});

		engine.getWindow().getRenderWindow().draw(text);

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

				engine.getWindow().getRenderWindow().draw(line, 2, sf::PrimitiveType::Lines);
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

			engine.getWindow().getRenderWindow().draw(circle);
		}

		if (hoveredNode)
		{
			sf::RectangleShape box;

			box.setPosition(mouse + sf::Vector2f(25.f, 25.f));
			box.setSize({ 320.f, 180.f });

			box.setFillColor(sf::Color(25, 25, 25, 240));
			box.setOutlineThickness(2.f);
			box.setOutlineColor(sf::Color::White);

			engine.getWindow().getRenderWindow().draw(box);

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

			engine.getWindow().getRenderWindow().draw(text);
		}
	}
	else
	{
		renderManager.renderAll(engine.getRenderer());

		sf::Text text(font);

		text.setCharacterSize(30);
		text.setFillColor(sf::Color::White);

		text.setString(
			FormatTime(runTime) +
			" / " +
			FormatTime(maxRunTime));

		sf::FloatRect bounds = text.getLocalBounds();

		text.setPosition({
			engine.getWindow().getRenderWindow().getSize().x / 2.f - bounds.size.x / 2.f,
			20.f
			});

		engine.getWindow().getRenderWindow().draw(text);

		text.setCharacterSize(30);
		text.setFillColor(sf::Color::White);

		text.setString(
			"HP: " +
			std::to_string(static_cast<int>(player.GetStats().health)) +
			" / " +
			std::to_string(static_cast<int>(player.GetStats().maxHealth)));

		text.setPosition({
			10.f,
			20.f
			});

		engine.getWindow().getRenderWindow().draw(text);
	}
}