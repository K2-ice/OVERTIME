#include "pch.h"
#include "Map.h"

namespace EngineL
{
	Map::Map() {}

	bool Map::isWallTileId(int tileId) const {

		return tileId == WallBrick || tileId == WallCrate || tileId == WallBush || tileId == WallRock;
	}


	bool Map::load(const int layout[height][width], const std::string& tileFolder) {

		if (!grassTextures[0].loadFromFile(tileFolder + "tile_01.png"))
			return false;

		if (!grassTextures[1].loadFromFile(tileFolder + "tile_02.png"))
			return false;

		if (!grassTextures[2].loadFromFile(tileFolder + "tile_03.png"))
			return false;

		if (!grassTextures[3].loadFromFile(tileFolder + "tile_04.png"))
			return false;

		if (!brickTexture.loadFromFile(tileFolder + "tile_42.png"))
			return false;

		if (!crateTexture.loadFromFile(tileFolder + "tile_130.png"))
			return false;

		if (!bushTexture.loadFromFile(tileFolder + "tile_183.png"))
			return false;

		if (!rockTexture.loadFromFile(tileFolder + "tile_185.png"))
			return false;

		for (int y = 0; y < height; y++) {

			for (int x = 0; x < width; x++) {

				tiles[y][x] = layout[y][x];
			}
		}

		return true;
	}

	const sf::Texture& Map::getGrassTexture(int x, int y) const {

		return grassTextures[(x + y) % 4];
	}

	const sf::Texture* Map::getWallTexture(int tileId) const {

		if (tileId == WallBrick)
			return &brickTexture;

		if (tileId == WallCrate)
			return &crateTexture;

		if (tileId == WallBush)
			return &bushTexture;

		if (tileId == WallRock)
			return &rockTexture;

		return nullptr;
	}

	void Map::drawTile(Renderer& renderer, const sf::Texture& texture, int x, int y) {

		sf::Sprite sprite(texture);

		sprite.setPosition(sf::Vector2f(
			static_cast<float>(x) * tileSize,
			static_cast<float>(y) * tileSize));

		renderer.drawSprite(sprite);
	}

	void Map::render(Renderer& renderer) {

		for (int y = 0; y < height; y++) {

			for (int x = 0; x < width; x++) {

				drawTile(renderer, getGrassTexture(x, y), x, y);

				const sf::Texture* wallTexture = getWallTexture(tiles[y][x]);

				if (wallTexture != nullptr)
					drawTile(renderer, *wallTexture, x, y);
			}
		}
	}

	bool Map::isWallArea(float x, float y, float areaWidth, float areaHeight) const {
		int tileLeft = static_cast<int>(x) / tileSize;
		int tileRight = static_cast<int>(x + areaWidth) / tileSize;
		int tileTop = static_cast<int>(y) / tileSize;
		int tileBottom = static_cast<int>(y + areaHeight) / tileSize;

		for (int ty = tileTop; ty <= tileBottom; ty++) {

			for (int tx = tileLeft; tx <= tileRight; tx++) {
				if (tx < 0 || tx >= width || ty < 0 || ty >= height)
					return true;

				if (isWallTileId(tiles[ty][tx]))
					return true;
			}
		}

		return false;
	}
}
