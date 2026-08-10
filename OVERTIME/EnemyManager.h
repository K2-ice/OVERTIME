#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "KamikazeEnemy.h"
#include "MeleeEnemy.h"
#include "BelierEnemy.h"
#include "Boss.h"
#include "Player.h"
#include "Bullet.h"
#include "UpdateManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "Map.h"

namespace EngineL {

	class EnemyManager {

	public:

		EnemyManager(UpdateManager& updateManager, RenderManager& renderManager, Player* player, Map* map);
		std::vector<Enemy*>& getEnemies();

		void update(float deltaTime, float runTime);
		void checkBulletCollisions(std::vector<Bullet*>& bullets);
		void checkPlayerCollision();
		void checkEnemyBulletCollisions();

		std::vector<sf::Vector2f> removeDeadEnemies();
		void clear();

	private:

		void spawnEnemy();
		void spawnBoss();
		void updateBossAttacks();
		void pushEnemyAwayFromPlayer(Enemy* enemy);

		UpdateManager& updateManager;
		RenderManager& renderManager;
		Player* player;
		Map* map;

		std::vector<Enemy*> enemies;
		std::vector<Bullet*> enemyBullets;
		sf::Vector2f getRandomSpawnPosition() const;

		float spawnTimer = 0.f;
		float spawnDelay = 1.f;
		float bossSpawnTime = 10.f;
		bool bossSpawned = false;

		Boss* boss = nullptr;
	};
}