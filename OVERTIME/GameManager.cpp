#include "pch.h"
#include "GameManager.h"
#include "SaveSystem.h"

#include <cmath>
#include <cstdlib>

GameManager::GameManager()
	: engine(1280, 720, "OVERTIME")
	, player(400.f, 300.f, &engine.getInputManager(), &map)
	, enemyManager(updateManager, renderManager, &player, &map)
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
				layout[y][x] = 42;
			}
			else
			{

				int variant = (x + y) % 4;
				layout[y][x] = 1 + variant;
			}
		}
	}

	for (int y = 6; y <= 12; y++)
	{
		if (y != 9)
			layout[y][15] = 42;
	}

	for (int y = 10; y <= 13; y++)
	{
		for (int x = 20; x <= 24; x++)
		{
			layout[y][x] = 42;
		}
	}

	for (int x = 25; x <= 35; x++)
	{
		layout[16][x] = 42;
	}
	for (int x = 25; x <= 35; x++)
	{
		layout[18][x] = 42;
	}

	layout[3][30] = 42;
	layout[3][32] = 42;
	layout[3][34] = 42;
	layout[20][8] = 42;
	layout[20][10] = 42;
	layout[20][12] = 42;

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
			player, weaponInventory);

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
		updateRecords();

		sceneManager.getSkillTreeScreen().setDuringRun(false);
		sceneManager.setState(GameState::SkillTree);
		showSkillTree = true;
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

	cleanupEnemies();
	collectSouls();
	collectWeaponPickups();
	cleanupBullets();

	if (player.GetStats().health <= 0)
	{
		updateRecords();
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
		weaponInventory.getCurrentWeapon()->startReload(player.GetStats().reloadSpeed);
	}
}

void GameManager::handleShooting()
{
	EngineL::Weapon* weapon = weaponInventory.getCurrentWeapon();

	if (player.wantsToShoot())
	{
		float x = player.getPosition().x;
		float y = player.getPosition().y;
		float dirX = player.getShootDirectionX();
		float dirY = player.getShootDirectionY();

		std::vector<EngineL::Bullet*> newBullets =
			weaponInventory.getCurrentWeapon()->fire(x, y, dirX, dirY, player.GetStats().damage, player.GetStats().attackSpeed, &map);

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

void GameManager::cleanupEnemies()
{
	std::vector<sf::Vector2f> deathPositions = enemyManager.removeDeadEnemies();

	killsThisRun += static_cast<int>(deathPositions.size());

	for (const sf::Vector2f& position : deathPositions)
	{
		EngineL::Soul* soul = new EngineL::Soul(position.x, position.y);

		souls.push_back(soul);
		updateManager.add(soul);
		renderManager.add(soul);

		EngineL::WeaponPickup* pickup = weaponInventory.tryDropWeapon(position.x, position.y, player.GetStats().hasShotgun, player.GetStats().hasSubmachinegun);

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
			int amount = soul->getValue() * player.GetStats().difficulty;

			player.addSouls(amount);
			soulsThisRun += amount;

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

void GameManager::updateRecords()
{
	if (runTime > bestTime)
		bestTime = runTime;

	if (killsThisRun > bestKills)
		bestKills = killsThisRun;

	if (soulsThisRun > bestSouls)
		bestSouls = soulsThisRun;
}

void GameManager::startNewRun()
{

	showSkillTree = false;
	maxRunTime = player.getMaxTime();
	runTime = 0.f;

	killsThisRun = 0;
	soulsThisRun = 0;

	player.setPosition(400.f, 300.f);
	player.resetDamageCooldown();
	player.resetRegenTimer();

	Stats& stats = player.GetStats();
	stats.health = stats.maxHealth;

	weaponInventory.reloadAll();
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
		player.hasSecondWeaponSlot(),
		bestTime,
		bestKills,
		bestSouls);
}