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
#include "Map.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Audio/Music.hpp>

class GameManager {
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
	void updateRecords();
	void updateMusic();
	void buildLevel1Layout(int layout[EngineL::Map::height][EngineL::Map::width]);
	void buildLevel2Layout(int layout[EngineL::Map::height][EngineL::Map::width]);
	void switchToLevel2();
	sf::View getGameView();
	sf::Vector2f getRandomPlayerSpawnPosition();

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

	bool isLevel2 = false;
	float level2Time = 60.f;

	float levelUpMessageTimer = 0.f;
	float levelUpMessageDuration = 3.f;

	int savedSouls = 0;


	int killsThisRun = 0;
	int soulsThisRun = 0;


	float bestTime = 0.f;
	int bestKills = 0;
	int bestSouls = 0;

	sf::Font font;
	HUD hud;
	SceneManager sceneManager;

	sf::Music music;
	std::string currentMusicPath;
};