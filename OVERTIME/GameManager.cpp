#include "pch.h"
#include "GameManager.h"
#include "SaveSystem.h"

#include <cmath>
#include <cstdlib>

GameManager::GameManager()
	: engine(1280, 720, "OVERTIME")
	, player(400.f, 300.f, &engine.getInputManager(), &map)
	, enemyManager(updateManager, renderManager, &player)
	, hud(font)
	, sceneManager(font)
{
	updateManager.add(&player);
	renderManager.add(&player);

	weaponInventory.reset(player.hasSecondWeaponSlot());

	if (!font.openFromFile("Assets/arial.ttf"))
		throw std::runtime_error("Failed to load font.");

	savedSouls = SaveSystem::load();
	sceneManager.getMainMenuScreen().setSavedSouls(savedSouls);

	int layout[EngineL::Map::height][EngineL::Map::width];

	for (int y = 0; y < EngineL::Map::height; y++)
	{
		for (int x = 0; x < EngineL::Map::width; x++)
		{
			bool border = (x == 0 || y == 0 || x == EngineL::Map::width - 1 || y == EngineL::Map::height - 1);

			if (border)
			{
				layout[y][x] = 42; // mur
			}
			else
			{
				// sol en herbe, avec un peu de variete (1,2,3,4)
				int variant = (x + y) % 4;
				layout[y][x] = 1 + variant;
			}
		}
	}

	// Quelques murs interieurs pour tester la collision
	layout[5][10] = 42;
	layout[5][11] = 42;
	layout[5][12] = 42;
	layout[10][20] = 42;
	layout[10][21] = 42;
	layout[15][5] = 42;
	layout[15][6] = 42;

	map.load(layout, "Assets/Tiles/");
}

GameManager::~GameManager()
{
	SaveSystem::save(player.getSouls());

	for (auto bullet : bullets)
	{
		updateManager.remove(bullet);
		renderManager.remove(bullet);
		delete bullet;
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

	enemyManager.clear();
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

sf::View GameManager::getGameView()
{
	sf::View view(engine.getWindow().getRenderWindow().getDefaultView());
	sf::Vector2f playerCenter = player.getPosition() + sf::Vector2f(16.f, 16.f);
	view.setCenter(playerCenter);
	return view;
}

void GameManager::update(float deltaTime)
{
	if (sceneManager.getState() != GameState::Playing)
	{
		bool startRun = sceneManager.updateMenus(
			engine.getInputManager(),
			engine.getWindow().getRenderWindow(),
			player);

		if (startRun)
		{
			if (sceneManager.consumeContinueRequested())
			{
				player.addSouls(savedSouls);
			}

			startNewRun();
		}

		return;
	}

	// etat "Playing"

	if (engine.getInputManager().isPausePressed())
	{
		sceneManager.setState(GameState::Paused);
		return;
	}

	sf::Vector2i mouse =
		engine.getInputManager().getMousePosition(
			engine.getWindow().getRenderWindow());

	sf::Vector2f worldMouse =
		engine.getWindow().getRenderWindow().mapPixelToCoords(mouse, getGameView());

	player.aimAt(worldMouse.x, worldMouse.y);

	runTime += deltaTime;

	if (runTime >= maxRunTime)
	{
		sceneManager.getSkillTreeScreen().setDuringRun(false);
		sceneManager.setState(GameState::SkillTree);
		return;
	}

	enemyManager.update(deltaTime, runTime);

	weaponInventory.handleSwitch(engine.getInputManager(), player.hasSecondWeaponSlot());
	handleReload();

	weaponInventory.getCurrentWeapon()->update(deltaTime);
	handleShooting();

	updateManager.updateAll(deltaTime);

	enemyManager.checkBulletCollisions(bullets);
	enemyManager.checkPlayerCollision();
	enemyManager.checkEnemyBulletCollisions();

	handleEnemyDeaths();
	collectSouls();
	collectWeaponPickups();
	cleanupBullets();

	if (player.GetStats().health <= 0)
	{
		sceneManager.setState(GameState::GameOver);
	}
}

void GameManager::render()
{
	if (sceneManager.getState() == GameState::Playing)
	{
		renderGameScene();
	}
	else if (sceneManager.getState() == GameState::Paused)
	{
		renderGameScene();
		sceneManager.renderCurrentMenu(
			engine.getWindow().getRenderWindow(),
			engine.getInputManager(),
			player,
			runTime);
	}
	else
	{
		sceneManager.renderCurrentMenu(
			engine.getWindow().getRenderWindow(),
			engine.getInputManager(),
			player,
			runTime);
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
	EngineL::Weapon* weapon = weaponInventory.getCurrentWeapon();

	if (player.wantsToShoot() && weapon->getCurrentAmmo() <= 0 && !weapon->isReloading())
	{
		weapon->startReload();
	}

	if (player.wantsToShoot())
	{
		float x = player.getPosition().x;
		float y = player.getPosition().y;
		float dirX = player.getShootDirectionX();
		float dirY = player.getShootDirectionY();

		std::vector<EngineL::Bullet*> newBullets =
			weapon->fire(x, y, dirX, dirY);

		for (EngineL::Bullet* bullet : newBullets)
		{
			bullets.push_back(bullet);
			updateManager.add(bullet);
			renderManager.add(bullet);
		}
	}
}

void GameManager::cleanupBullets()
{
	float mapPixelWidth = static_cast<float>(EngineL::Map::width * EngineL::Map::tileSize);
	float mapPixelHeight = static_cast<float>(EngineL::Map::height * EngineL::Map::tileSize);

	for (int i = 0; i < bullets.size(); i++)
	{
		EngineL::Bullet* bullet = bullets[i];

		float x = bullet->getPosition().x;
		float y = bullet->getPosition().y;

		bool outOfScreen = x < 0.f || x > mapPixelWidth || y < 0.f || y > mapPixelHeight;

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

void GameManager::handleEnemyDeaths()
{
	std::vector<sf::Vector2f> deathPositions = enemyManager.removeDeadEnemies();

	for (const sf::Vector2f& position : deathPositions)
	{
		EngineL::Soul* soul = new EngineL::Soul(position.x, position.y);

		souls.push_back(soul);
		updateManager.add(soul);
		renderManager.add(soul);

		EngineL::WeaponPickup* pickup = weaponInventory.tryDropWeapon(position.x, position.y);

		if (pickup != nullptr)
		{
			weaponPickups.push_back(pickup);
			updateManager.add(pickup);
			renderManager.add(pickup);
		}
	}
}

void GameManager::collectSouls()
{
	for (int i = 0; i < souls.size(); i++)
	{
		EngineL::Soul* soul = souls[i];

		if (EngineL::CollisionManager::checkCollision(&player, soul))
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

		if (EngineL::CollisionManager::checkCollision(&player, pickup))
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

void GameManager::startNewRun()
{
	runTime = 0.f;

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

	enemyManager.clear();
}

void GameManager::renderGameScene()
{
	sf::RenderWindow& window = engine.getWindow().getRenderWindow();

	sf::View gameView = getGameView();
	window.setView(gameView);

	map.render(engine.getRenderer());
	renderManager.renderAll(engine.getRenderer());

	window.setView(window.getDefaultView());

	hud.draw(
		window,
		player,
		weaponInventory.getCurrentWeapon(),
		runTime,
		maxRunTime,
		player.hasSecondWeaponSlot());
}
