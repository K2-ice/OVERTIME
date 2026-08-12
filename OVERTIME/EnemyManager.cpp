#include "pch.h"
#include "EnemyManager.h"

#include <cmath>
#include <cstdlib>

namespace EngineL {

	EnemyManager::EnemyManager(UpdateManager& updateManager, RenderManager& renderManager, Player* player, Map* map)
		: updateManager(updateManager)
		, renderManager(renderManager)
		, player(player),
		map(map) {
	}

	std::vector<Enemy*>& EnemyManager::getEnemies() {
		return enemies;
	}

	void EnemyManager::update(float deltaTime, float runTime, bool isLevel2) {

		currentIsLevel2 = isLevel2;

		spawnTimer += deltaTime;

		float currentSpawnDelay = currentIsLevel2 ? 0.7f : spawnDelay;

		if (spawnTimer >= currentSpawnDelay) {
			spawnTimer = 0.f;
			spawnEnemy();
		}

		if (!bossSpawned && runTime >= bossSpawnTime) {
			spawnBoss();
		}

		updateBossAttacks();
	}

	void EnemyManager::spawnEnemy() {

		sf::Vector2f spawnPosition = getRandomSpawnPosition();

		float x = spawnPosition.x;
		float y = spawnPosition.y;

		int roll = rand() % 100;

		Enemy* enemy = nullptr;

		if (!currentIsLevel2) {


			if (roll < 45) {
				enemy = new Enemy(x, y, player, player->GetStats().difficulty, map);
			}
			else if (roll < 65) {
				enemy = new KamikazeEnemy(x, y, player, player->GetStats().difficulty, map);
			}
			else if (roll < 85) {
				enemy = new MeleeEnemy(x, y, player, player->GetStats().difficulty, map);
			}
			else {
				enemy = new BelierEnemy(x, y, player, player->GetStats().difficulty, map);
			}
		}

		else {


			if (roll < 15) {
				enemy = new Enemy(x, y, player, player->GetStats().difficulty, map);
			}
			else if (roll < 50) {
				enemy = new KamikazeEnemy(x, y, player, player->GetStats().difficulty, map);
			}
			else if (roll < 65) {
				enemy = new MeleeEnemy(x, y, player, player->GetStats().difficulty, map);
			}
			else {
				enemy = new BelierEnemy(x, y, player, player->GetStats().difficulty, map);
			}
		}

		enemies.push_back(enemy);

		updateManager.add(enemy);
		renderManager.add(enemy);
	}

	void EnemyManager::spawnBoss() {

		bossSpawned = true;

		sf::Vector2f spawnPosition = getRandomSpawnPosition();

		boss = new Boss(spawnPosition.x, spawnPosition.y, player, player->GetStats().difficulty, map);

		enemies.push_back(boss);

		updateManager.add(boss);
		renderManager.add(boss);
	}

	void EnemyManager::updateBossAttacks() {

		if (boss == nullptr || !boss->isAlive())
			return;

		if (boss->wantsToFire()) {

			std::vector<Bullet*> newBullets = boss->fire(map);

			for (Bullet* bullet : newBullets) {
				enemyBullets.push_back(bullet);
				updateManager.add(bullet);
				renderManager.add(bullet);
			}
		}
	}

	void EnemyManager::checkBulletCollisions(std::vector<Bullet*>& bullets) {

		for (int b = 0; b < static_cast<int>(bullets.size()); b++) {

			Bullet* bullet = bullets[b];

			bool bulletDestroyed = false;

			for (int e = 0; e < static_cast<int>(enemies.size()); e++) {

				Enemy* enemy = enemies[e];

				if (!enemy->isAlive())
					continue;

				if (CollisionManager::checkCollision(bullet, enemy)) {

					float damage = bullet->getDamage();

					float rollCrit =
						static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

					bool isCrit =
						rollCrit < player->GetStats().critChance;

					if (isCrit)
					{
						damage *= player->GetStats().critDamage;
					}

					if (player->GetStats().health < player->GetStats().maxHealth)
					{
						float lifestealAmount =
							damage * player->GetStats().lifesteal;

						float missingHealth =
							player->GetStats().maxHealth -
							player->GetStats().health;

						if (lifestealAmount > missingHealth)
							lifestealAmount = missingHealth;

						player->takeDamage(
							-static_cast<int>(lifestealAmount));
					}

					enemy->takeDamage(static_cast<int>(damage));

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

	void EnemyManager::checkEnemyBulletCollisions() {

		for (int b = 0; b < enemyBullets.size(); b++) {

			Bullet* bullet = enemyBullets[b];

			bool remove = false;

			if (CollisionManager::checkCollision(bullet, player)) {

				if (player->canTakeDamage()) {

					player->takeDamage(static_cast<int>(bullet->getDamage()));
					player->resetDamageCooldown();
				}

				remove = true;
			}
			else if (bullet->hasHitWall()) {

				remove = true;
			}

			if (remove) {

				updateManager.remove(bullet);
				renderManager.remove(bullet);

				delete bullet;

				enemyBullets.erase(enemyBullets.begin() + b);
				b--;
			}
		}
	}


	void EnemyManager::checkPlayerCollision() {

		for (Enemy* enemy : enemies) {

			if (CollisionManager::checkCollision(enemy, player)) {


				if (player->canTakeDamage()) {

					player->takeDamage(static_cast<int>(enemy->GetStats().damage));
					player->resetDamageCooldown();
				}

				if (enemy->explodesOnContact()) {

					enemy->takeDamage(9999);
				}

				else {

					pushEnemyAwayFromPlayer(enemy);
				}
			}
		}
	}

	void EnemyManager::pushEnemyAwayFromPlayer(Enemy* enemy) {
		sf::Vector2f enemyPos = enemy->getPosition();
		sf::Vector2f playerPos = player->getPosition();

		float dx = enemyPos.x - playerPos.x;
		float dy = enemyPos.y - playerPos.y;

		float length = std::sqrt(dx * dx + dy * dy);

		if (length == 0.f) {
			dx = 1.f;
			dy = 0.f;
			length = 1.f;
		}

		dx /= length;
		dy /= length;

		float pushDistance = 3.f;

		enemy->setPosition(enemyPos.x + dx * pushDistance, enemyPos.y + dy * pushDistance);
	}

	sf::Vector2f EnemyManager::getRandomSpawnPosition() const {

		float mapPixelWidth = static_cast<float>(Map::width * Map::tileSize);
		float mapPixelHeight = static_cast<float>(Map::height * Map::tileSize);

		float x = static_cast<float>(rand() % static_cast<int>(mapPixelWidth - 32.f));
		float y = static_cast<float>(rand() % static_cast<int>(mapPixelHeight - 32.f));

		if (map != nullptr && map->isWallArea(x, y, 32.f, 32.f)) {
			return player->getPosition();
		}

		return sf::Vector2f(x, y);
	}

	std::vector<sf::Vector2f> EnemyManager::removeDeadEnemies() {
		std::vector<sf::Vector2f> deathPositions;

		for (int i = 0; i < enemies.size(); i++) {
			Enemy* enemy = enemies[i];

			if (!enemy->isAlive()) {
				deathPositions.push_back(enemy->getPosition());

				if (enemy == boss)
					boss = nullptr;

				updateManager.remove(enemy);
				renderManager.remove(enemy);

				delete enemy;

				enemies.erase(enemies.begin() + i);
				i--;
			}
		}

		return deathPositions;
	}

	void EnemyManager::clear() {
		for (Enemy* enemy : enemies) {

			updateManager.remove(enemy);
			renderManager.remove(enemy);
			delete enemy;
		}

		enemies.clear();

		for (Bullet* bullet : enemyBullets) {

			updateManager.remove(bullet);
			renderManager.remove(bullet);
			delete bullet;
		}

		enemyBullets.clear();

		boss = nullptr;
		bossSpawned = false;
	}
}