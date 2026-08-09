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
#include <iostream>
#include "SaveSystem.h"
#include "Map.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>

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
	void spawnEnemy(float deltaTime);
	void cleanupEnemies();
	void checkBulletEnemyCollisions();
	void startNewRun();
	void renderGameScene();
	void collectSouls();
	void collectWeaponPickups();

	sf::View getGameView();

	EngineL::GameEngine engine;
	EngineL::Map map;
	EngineL::Player player;
	EngineL::UpdateManager updateManager;
	EngineL::RenderManager renderManager;
	EngineL::EnemyManager enemyManager;

	std::vector<EngineL::Bullet*> bullets;
	std::vector<EngineL::Soul*> souls;
	std::vector<EngineL::WeaponPickup*> weaponPickups;

	WeaponInventory weaponInventory;

	float spawnTimer = 0.f;
	float spawnDelay = 1.f;
	float death = 0.f;
	bool showSkillTree = true;

	float runTime = 0.f;
	float maxRunTime = player.getMaxTime();

	int	savedSouls = 0;

	sf::Font font;

	HUD hud;
	SceneManager sceneManager;
};