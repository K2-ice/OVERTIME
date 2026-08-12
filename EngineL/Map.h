#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Renderer.h"
#include "pch.h"

namespace EngineL {

	class Map {

	public:

		static const int width = 40;
		static const int height = 24;
		static const int tileSize = 64;

		static const int WallBrick = 42;
		static const int WallCrate = 130;
		static const int WallBush = 183;
		static const int WallRock = 185;

		Map();

		bool load(const int layout[height][width], const std::string& tileFolder);
		void render(Renderer& renderer);

		bool isWallArea(float x, float y, float areaWidth, float areaHeight) const;

	private:

		bool isWallTileId(int tileId) const;

		
		void drawTile(Renderer& renderer, const sf::Texture& texture, int x, int y);

		const sf::Texture& getGrassTexture(int x, int y) const;

		const sf::Texture* getWallTexture(int tileId) const;

		sf::Texture grassTextures[4];
		sf::Texture brickTexture;
		sf::Texture crateTexture;
		sf::Texture bushTexture;
		sf::Texture rockTexture;

		int tiles[height][width] = {};
	};
}
