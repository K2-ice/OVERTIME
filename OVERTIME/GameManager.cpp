#include "pch.h"
#include "GameManager.h"

#include <cmath>
#include <cstdlib>

GameManager::GameManager()
	: engine(1280, 720, "OVERTIME")
	, player(400.f, 300.f, &engine.getInputManager())
	, hud(font)
	, skillTreeScreen(font)
{
	updateManager.add(&player);
	renderManager.add(&player);

	weaponInventory.reset(player.hasSecondWeaponSlot());

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

	for (auto pickup : weaponPickups)
	{
		updateManager.remove(pickup);
		renderManager.remove(pickup);
		delete pickup;
	}
}

void GameManager::run()
{
	while (engine.isRunning())
	{
		float deltaTime = engine.beginFrame();

		update(deltaTime);
		render();

		engine.endFrame();
	}
}

void GameManager::update(float deltaTime)
{
	if (showSkillTree)
	{
		skillTreeScreen.handleInput(
			engine.getInputManager(),
			engine.getWindow().getRenderWindow(),
			player);

		if (engine.getInputManager().isStartPressed())
		{
			startNewRun();
		}

		return;
	}

	sf::Vector2i mouse =
		engine.getInputManager().getMousePosition(
			engine.getWindow().getRenderWindow());

	player.aimAt(
		static_cast<float>(mouse.x),
		static_cast<float>(mouse.y));

	runTime += deltaTime;

	if (runTime >= maxRunTime)
	{
		showSkillTree = true;
		return;
	}

	spawnTimer += deltaTime;

	if (spawnTimer >= spawnDelay)
	{
		spawnTimer = 0.f;
		spawnEnemy();
	}

	weaponInventory.handleSwitch(engine.getInputManager(), player.hasSecondWeaponSlot());
	handleReload();

	weaponInventory.getCurrentWeapon()->update(deltaTime);
	handleShooting();

	updateManager.updateAll(deltaTime);

	checkBulletEnemyCollisions();
	checkEnemyPlayerCollisions();
	cleanupEnemies();
	collectSouls();
	collectWeaponPickups();
	cleanupBullets();
}

void GameManager::render()
{
	if (showSkillTree)
	{
		skillTreeScreen.render(engine.getWindow().getRenderWindow(), player);
	}
	else
	{
		renderGameScene();
	}
}

void GameManager::handleReload()
{
	if (engine.getInputManager().isReloadPressed())
	{
		weaponInventory.getCurrentWeapon()->startReload();
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

		std::vector<EngineL::Bullet*> newBullets =
			weaponInventory.getCurrentWeapon()->fire(x, y, dirX, dirY);

		for (EngineL::Bullet* bullet : newBullets)
		{
			bullets.push_back(bullet);
			updateManager.add(bullet);
			renderManager.add(bullet);
		}
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
					static_cast<int>(bullet->getDamage()));

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

			EngineL::WeaponPickup* pickup =
				weaponInventory.tryDropWeapon(
					enemy->getPosition().x,
					enemy->getPosition().y);

			if (pickup != nullptr)
			{
				weaponPickups.push_back(pickup);
				updateManager.add(pickup);
				renderManager.add(pickup);
			}

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

void GameManager::collectWeaponPickups()
{
	for (int i = 0; i < weaponPickups.size(); i++)
	{
		EngineL::WeaponPickup* pickup = weaponPickups[i];

		if (player.getBounds().findIntersection(pickup->getBounds()))
		{
			EngineL::Weapon* weapon = weaponInventory.getWeaponById(pickup->getWeaponId());

			if (weapon != nullptr && !weaponInventory.isInInventory(weapon))
			{
				weaponInventory.equipPickup(weapon);
			}

			updateManager.remove(pickup);
			renderManager.remove(pickup);

			delete pickup;

			weaponPickups.erase(weaponPickups.begin() + i);

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

	player.setPosition(400.f, 300.f);

	Stats& stats = player.GetStats();
	stats.health = stats.maxHealth;

	weaponInventory.reset(player.hasSecondWeaponSlot());

	for (auto bullet : bullets)
	{
		updateManager.remove(bullet);
		renderManager.remove(bullet);
		delete bullet;
	}
	bullets.clear();

	for (auto enemy : enemies)
	{
		updateManager.remove(enemy);
		renderManager.remove(enemy);
		delete enemy;
	}
	enemies.clear();

	for (auto soul : souls)
	{
		updateManager.remove(soul);
		renderManager.remove(soul);
		delete soul;
	}
	souls.clear();

	for (auto pickup : weaponPickups)
	{
		updateManager.remove(pickup);
		renderManager.remove(pickup);
		delete pickup;
	}
	weaponPickups.clear();
}

void GameManager::renderGameScene()
{
	renderManager.renderAll(engine.getRenderer());

	hud.draw(
		engine.getWindow().getRenderWindow(),
		player,
		weaponInventory.getCurrentWeapon(),
		runTime,
		maxRunTime,
		player.hasSecondWeaponSlot());
}