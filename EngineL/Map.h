#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Renderer.h"
#include "pch.h"

namespace EngineL {

	class Map {

	public:

		static const int width = 40;
		static const int height = 24;
		static const int tileSize = 64;

		Map();

		bool load(const int layout[height][width], const std::string& tileFolder);
		void render(Renderer& renderer);

		bool isWallArea(float x, float y, float areaWidth, float areaHeight) const;

	private:

		bool isWallTileId(int tileId) const;

		std::vector<std::unique_ptr<sf::Texture>> textures;
		std::vector<sf::Sprite> sprites;
		int tiles[height][width] = {};
	};
}