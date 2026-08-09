#include "pch.h"
#include "Map.h"
#include <sstream>
#include <iomanip>

namespace EngineL
{
	Map::Map()
	{
	}

	bool Map::isWallTileId(int tileId) const
	{
		
		return tileId == 42 || tileId == 13;
	}

	bool Map::load(const int layout[height][width], const std::string& tileFolder)
	{
		textures.clear();
		sprites.clear();

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				tiles[y][x] = layout[y][x];

				int tileId = layout[y][x];

				if (tileId <= 0)
					continue;

				std::ostringstream fileName;
				fileName << tileFolder << "tile_" << std::setfill('0') << std::setw(2) << tileId << ".png";

				std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

				if (!texture->loadFromFile(fileName.str()))
					return false;

				textures.push_back(std::move(texture));

				sf::Sprite sprite(*textures.back());
				sprite.setPosition(sf::Vector2f(
					static_cast<float>(x) * tileSize,
					static_cast<float>(y) * tileSize));

				sprites.push_back(sprite);
			}
		}

		return true;
	}

	void Map::render(Renderer& renderer)
	{
		for (const sf::Sprite& sprite : sprites)
		{
			renderer.drawSprite(sprite);
		}
	}

	bool Map::isWallArea(float x, float y, float areaWidth, float areaHeight) const
	{
		int tileLeft = static_cast<int>(x) / tileSize;
		int tileRight = static_cast<int>(x + areaWidth) / tileSize;
		int tileTop = static_cast<int>(y) / tileSize;
		int tileBottom = static_cast<int>(y + areaHeight) / tileSize;

		for (int ty = tileTop; ty <= tileBottom; ty++)
		{
			for (int tx = tileLeft; tx <= tileRight; tx++)
			{
				if (tx < 0 || tx >= width || ty < 0 || ty >= height)
					return true;

				if (isWallTileId(tiles[ty][tx]))
					return true;
			}
		}

		return false;
	}
}