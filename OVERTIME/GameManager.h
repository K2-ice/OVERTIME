#pragma once

#include <vector>
#include <string>

#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Soul.h"
#include "WeaponPickup.h"
#include "WeaponInventory.h"

#include "UpdateManager.h"
#include "RenderManager.h"
#include "SkillTreeScreen.h"
#include "HUD.h"
#include <iostream>
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

	void spawnEnemy();
	void cleanupBullets();

	void startNewRun();
	void renderGameScene();

	void checkBulletEnemyCollisions();
	void cleanupEnemies();
	void collectSouls();
	void collectWeaponPickups();
	void checkEnemyPlayerCollisions();

	EngineL::GameEngine engine;

	EngineL::Player player;

	EngineL::UpdateManager updateManager;
	EngineL::RenderManager renderManager;

	std::vector<EngineL::Bullet*> bullets;
	std::vector<EngineL::Enemy*> enemies;
	std::vector<EngineL::Soul*> souls;
	std::vector<EngineL::WeaponPickup*> weaponPickups;

	WeaponInventory weaponInventory;

	float spawnTimer = 0.f;
	float spawnDelay = 1.f;
	float death = 0.f;
	bool showSkillTree = true;

	float runTime = 0.f;
	float maxRunTime = player.getMaxTime();

	sf::Font font;
	HUD hud;
	SkillTreeScreen skillTreeScreen;
};