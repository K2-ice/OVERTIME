#pragma once

#include <vector>

#include "GameEngine.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Soul.h"

#include "UpdateManager.h"
#include "RenderManager.h"
#include "SkillTree.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class GameManager
{
public:
	GameManager();
	~GameManager();

	void run();

private:
	void handleShooting();

	void spawnEnemy();
	void cleanupBullets();

	EngineL::GameEngine engine;

	EngineL::Player player;

	EngineL::UpdateManager updateManager;
	EngineL::RenderManager renderManager;

	std::vector<EngineL::Bullet*> bullets;
	std::vector<EngineL::Enemy*> enemies;
	std::vector<EngineL::Soul*> souls;

	float spawnTimer = 0.f;
	float spawnDelay = 1.f;

	SkillTree skillTree;

	bool showSkillTree = false;

	float runTime = 0.f;
	float maxRunTime = 10.f;

	sf::Font font;

	void startNewRun();
	bool mouseHeld = false;
	void handleSkillTree();
	void renderSkillTree();
	std::string FormatTime(float seconds);

	void checkBulletEnemyCollisions();
	void cleanupEnemies();
	void collectSouls();
	void checkEnemyPlayerCollisions();
};