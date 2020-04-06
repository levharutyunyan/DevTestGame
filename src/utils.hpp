#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <SFML/Graphics.hpp>

namespace utils
{
	static const float GEM_SIZE = 25;
	static const int WINDOW_WIDTH = 800;
	static const int WINDOW_HEIGHT = 600;
	static const int TILE_WIDTH = 50;
	static const int TILE_HEIGHT = 50;
	static const int GEM_HEIGHT = 35;
	static const int GEM_WIDTH = 35;

	static const float GRID_X_POS = static_cast<int>(WINDOW_WIDTH / 5);
	static const float GRID_Y_POS = static_cast<int>(WINDOW_HEIGHT / 4);

	static const double GRID_X_SCALE = 0.5;
	static const double GRID_Y_SCALE = 0.5;
	
	static const int MAX_FRAMERATE_LIMIT = 60;

	static const sf::Vector2i NULL_POS(-1, -1);

	enum class GemType : int
	{
		RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4
	};
	static const std::vector<GemType> ALL_GEM_TYPES_VECTOR
	{ 
		GemType::RED, GemType::ORANGE, GemType::GREEN, GemType::BLUE, GemType::VIOLET 
	};
	
	enum class TileType : int
	{
		GreyTile = 0, WhiteTile = 1
	};

	static const std::vector<TileType> ALL_TILES_TYPES_VECTOR
	{ 
		TileType::GreyTile, TileType::WhiteTile
	};

	enum class PowerUpType : int
	{
		H_BOMB = 0, V_BOMB = 1, R_BOMB = 2
	};

	static const std::vector<PowerUpType> ALL_POWER_UPS_TYPES_VECTOR
	{
		PowerUpType::H_BOMB, PowerUpType::V_BOMB, PowerUpType::R_BOMB
	};

	using GemPair = std::pair<sf::Vector2i, sf::Vector2i>;
	static const GemPair NULL_GEM_PAIR{{-1, -1}, {-1, -1}};

	using Pattern = std::vector<sf::Vector2i>;
	static const Pattern NULL_PATTERN{};
	
	// static std::vector<Pattern> POSSIBLE_PATTERNS
	// {
	// 	//Vertical pattenrs
	// 	{{-2, 0}, {-1, 0}, {0, 0}},
	// 	{{-1, 0}, {0, 0}, {1, 0}},
	// 	{{0, 0}, {1, 0}, {2, 0}},
	// 	//Horizontal patterns
	// 	{{0, -2}, {0, -1}, {0, 0}},
	// 	{{0, -2}, {0, -1}, {0, 0}},
	// 	{{0, -2}, {0, -1}, {0, 0}},
	// 	{{-1, 0}, {-1, 0}, {0, 0}, {1, 0}},
	// };

	

	// Filenames for resources
	static const std::string GREY_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_1.png");
	static const std::string WHITE_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_2.png");
	static const std::string GRID_BACKGROUND_FILENAME("C:/Users/Levonog/Desktop/images/background.jpg");
	static const std::string CONFIG_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/src/config.json");
	// Gems
	static const std::string RED_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/red.png");
	static const std::string ORANGE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/orange.png");
	static const std::string GREEN_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/green.png");
	static const std::string BLUE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/blue.png");
	static const std::string VIOLET_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/violet.png");

	// functions
	std::string getGemImageFilename(GemType gem_type);
	void printPattern(const Pattern& pattern);
	bool isValidPattern(const Pattern& pattern);
}

#endif // __UTILS_HPP__ 