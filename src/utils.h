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

	static const int MINIMUM_GEMS_COUNT = 0;
	static const int MAXIMUM_GEMS_COUNT = 100;
	static const int MINIMUM_BOMBS_COUNT = 100;
	static const int MAXIMUM_BOMBS_COUNT = 200;
	enum class PieceType : int
	{
		RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4,
		H_BOMB = MINIMUM_BOMBS_COUNT + 1, V_BOMB = MINIMUM_BOMBS_COUNT + 2, R_BOMB = MINIMUM_BOMBS_COUNT + 3,
		UNKNOWN = 1000, NONE = 1001,

	};
	static const std::vector<PieceType> ALL_PIECE_TYPES_VECTOR
	{ 
		PieceType::RED, PieceType::ORANGE, PieceType::GREEN, PieceType::BLUE, PieceType::VIOLET, 
		PieceType::H_BOMB, PieceType::V_BOMB, PieceType::R_BOMB
	};
	
	enum class TileType : int
	{
		GREY_TILE = 0, WHITE_TILE = 1, AFFECTED_TILE = 2, SELECTED_TILE = 3
	};

	static const std::vector<TileType> ALL_TILE_TYPES_VECTOR
	{ 
		TileType::GREY_TILE, TileType::WHITE_TILE, TileType::AFFECTED_TILE, TileType::SELECTED_TILE
	};

	using GemPair = std::pair<sf::Vector2i, sf::Vector2i>;
	static const GemPair NULL_GEM_PAIR{{-1, -1}, {-1, -1}};

	enum class PatternType : int
	{
		NONE = 0, MATCH = 1, H_BOMB = 2, V_BOMB = 3, R_BOMB = 4
	};
	static const std::vector<PatternType> ALL_PATTERN_TYPES_VECTOR
	{
		PatternType::NONE, PatternType::MATCH, PatternType::H_BOMB, PatternType::V_BOMB, PatternType::R_BOMB
	};
	using Pattern = std::vector<sf::Vector2i>;
	static const Pattern NULL_PATTERN{};
	static const std::vector<sf::Vector2i> H_PATTERN_DIR{ { -1, 0 }, { 1, 0 } };
	static const std::vector<sf::Vector2i> V_PATTERN_DIR{ { 0, -1 }, { 0, 1 } };
	static const std::vector<sf::Vector2i> DIAG_PATTERN_DIR{ { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	static const std::vector<sf::Vector2i> H_DIAG_PATTERN_DIR{ { -1, 0 }, { 1, 0 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	static const std::vector<sf::Vector2i> V_DIAG_PATTERN_DIR{ { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	static const std::vector<sf::Vector2i> ALL_PATTERN_DIR{ { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }, { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
	// Filenames for resources
	static const std::string CONFIG_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/src/config.json");

	static const std::string GREY_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/grey_tile.png");
	static const std::string WHITE_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/white_tile.png");
	static const std::string SELECTED_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/selected_tile.png");
	static const std::string AFFECTED_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/affected_tile.png");
	// Gems
	static const std::string NONE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/none.png");
	static const std::string UNKNOWN_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/unknown.png");
	static const std::string RED_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/red.png");
	static const std::string ORANGE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/orange.png");
	static const std::string GREEN_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/green.png");
	static const std::string BLUE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/blue.png");
	static const std::string VIOLET_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/violet.png");
	//BOMBS
	static const std::string H_BOMB_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/h_bomb.png");
	static const std::string V_BOMB_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/v_bomb.png");
	static const std::string R_BOMB_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/bomb.png");


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
	PieceType randomGemType(const std::vector<std::pair<utils::PieceType, int>>& objs, int figures_colors_count);
	std::string getPieceImageFilename(PieceType pieceType);
	PieceType toPieceType(PatternType patternType);
	std::string getTileImageFilename(TileType pieceType);
	PieceType getPieceType(const std::string& gemName);
	bool isValidPattern(const Pattern& pattern);
}

#endif // __UTILS_HPP__ 