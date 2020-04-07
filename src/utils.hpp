#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <SFML/Graphics.hpp>

namespace utils
{
	static const int MIN_COLUMNS = 7;
	static const int MAX_COLUMNS = 10;
	static const int MIN_ROWS = 7;
	static const int MAX_ROWS = 10;
	static const int MIN_FIGURES_COUNT = 3;
	static const int MAX_FIGURES_COUNT = 5;
	static const int MIN_OBJECTIVES_COUNT = 1;
	static const int MAX_OBJECTIVES_COUNT = 3;

	static const int TILE_WIDTH = 50;
	static const int TILE_HEIGHT = 50;

	static const int GEM_WIDTH = 35;
	static const int GEM_HEIGHT = 35;

	static const int WINDOW_WIDTH = 800;
	static const int WINDOW_HEIGHT = 600;

	static const int OBJECTIVES_WIDTH = WINDOW_WIDTH;
	static const int OBJECTIVES_HEIGHT = WINDOW_HEIGHT / 7;
	static const float OBJECTIVES_X_POS = 0;
	static const float OBJECTIVES_Y_POS = 0;

	static const int TURNS_WIDTH = OBJECTIVES_WIDTH / 4;
	static const int TURNS_HEIGHT = OBJECTIVES_HEIGHT;
	static const float TURNS_X_POS = 0;
	static const float TURNS_Y_POS = static_cast<float>(WINDOW_WIDTH / 2 - TURNS_WIDTH / 2);

	static const float GRID_X_POS = static_cast<float>(WINDOW_WIDTH / 2 - (MAX_ROWS / 3) * TILE_WIDTH);
	static const float GRID_Y_POS = static_cast<float>((WINDOW_HEIGHT - OBJECTIVES_HEIGHT) / 2 - (MAX_COLUMNS / 4) * TILE_WIDTH);
	static const double GRID_X_SCALE = 0.5;
	static const double GRID_Y_SCALE = 0.5;

	static const int GAME_OVER_WINDOW_HEIGHT = WINDOW_HEIGHT / 2;
	static const int GAME_OVER_WINDOW_WIDTH = WINDOW_WIDTH / 2;
	static const float GAME_OVER_WINDOW_X_POS = (WINDOW_WIDTH - GAME_OVER_WINDOW_WIDTH) / 2;
	static const float GAME_OVER_WINDOW_Y_POS = (WINDOW_HEIGHT - GAME_OVER_WINDOW_HEIGHT) / 2;
	

	static const int MAX_FRAMERATE_LIMIT = 60;

	static const sf::Vector2i NULL_POS(-1, -1);

	enum class GemType : int
	{
		RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4, UNKNOWN = 5
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
	static const std::string CONFIG_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/src/config.json");

	static const std::string GREY_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_1.png");
	static const std::string WHITE_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_2.png");
	//static const std::string GRID_BACKGROUND_FILENAME("C:/Users/Levonog/Desktop/images/background.jpg");
	// Gems
	static const std::string RED_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/red.png");
	static const std::string ORANGE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/orange.png");
	static const std::string GREEN_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/green.png");
	static const std::string BLUE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/blue.png");
	static const std::string VIOLET_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/violet.png");



	static const sf::Color GAME_CONTROLLER_COLOR(50, 71, 76, 255);
	// Objectives
	static const sf::Color OBJECTIVES_COLOR(143, 143, 143, 255);
	static const std::string OBJECTIVES_FONT_FILENAME("C:/Windows/Fonts/ARIAL.TTF");
	static const sf::Color OBJECTIVES_FONT_COLOR(sf::Color::White);
	static const int OBJECTIVES_FONT_SIZE = 24;
	static sf::Vector2f OBJECTIVES_OFFSET(45, 10);
	
	// Turns
	static const sf::Color TURNS_COLOR(100, 100, 100, 255);
	static const std::string TURNS_FONT_FILENAME(OBJECTIVES_FONT_FILENAME);
	static const sf::Color TURNS_FONT_COLOR(OBJECTIVES_FONT_COLOR);
	static const int TURNS_FONT_SIZE = 50;
	static sf::Vector2f TURNS_OFFSET(25, 10);
	
	// GameOverWindow
	static const sf::Color GAME_OVER_COLOR(44, 49, 58, 255);
	static const std::string GAME_OVER_FONT_FILENAME(OBJECTIVES_FONT_FILENAME);
	static const sf::Color GAME_OVER_FONT_COLOR(OBJECTIVES_FONT_COLOR);
	static const int GAME_OVER_FONT_SIZE = 50;
	static const std::string GAME_OVER_WIN_TEXT("Game Complete");
	static const std::string GAME_OVER_LOSE_TEXT("Game Failed");
	
	// functions
	std::string getGemImageFilename(GemType gemType);
	GemType getGemType(const std::string& gemName);
	//void printPattern(const Pattern& pattern);
	bool isValidPattern(const Pattern& pattern);
}

#endif // __UTILS_HPP__ 