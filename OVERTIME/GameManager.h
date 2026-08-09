#pragma once
#include <vector>
#include <string>

#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Soul.h"
#include "WeaponPickup.h"
#include "WeaponInventory.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "HUD.h"
#include "SaveSystem.h"

#include <SFML/Graphics/Font.hpp>

class GameManager
{
public:
	GameManager();
	~GameManager();

	void run();

private:

	void update(float deltaTime);
	void render();

	void handleShooting();
	void handleReload();

	void cleanupBullets();
	void handleEnemyDeaths();

	void startNewRun();
	void renderGameScene();

	void collectSouls();
	void collectWeaponPickups();

	EngineL::GameEngine engine;
	EngineL::Player player;
	EngineL::UpdateManager updateManager;
	EngineL::RenderManager renderManager;

	EngineL::EnemyManager enemyManager;

	std::vector<EngineL::Bullet*> bullets;
	std::vector<EngineL::Soul*> souls;
	std::vector<EngineL::WeaponPickup*> weaponPickups;

	WeaponInventory weaponInventory;

	float runTime = 0.f;
	float maxRunTime = 10.f;
	int	savedSouls = 0;

	sf::Font font;
	HUD hud;
	SceneManager sceneManager;
};