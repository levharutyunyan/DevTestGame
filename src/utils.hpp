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

	static const float GRID_X_POS = static_cast<int>(WINDOW_WIDTH / 5);
	static const float GRID_Y_POS = static_cast<int>(WINDOW_HEIGHT / 3);

	static const double GRID_X_SCALE = 0.5;
	static const double GRID_Y_SCALE = 0.5;
	
	static const int MAX_FRAMERATE_LIMIT = 60;

	static const sf::Vector2i NULL_POS(-1, -1);

	enum class GemType : int
	{
		RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, 
		VIOLET = 4
		// TRANSPARENT = 8, BLACK = 9
	};
	static std::vector<GemType> ALL_GEM_TYPES_VECTOR
	{ 
		GemType::RED, GemType::ORANGE, GemType::GREEN, GemType::BLUE, GemType::VIOLET 
	};

	using Pattern = std::vector<sf::Vector2i>;
	static Pattern NULL_PATTERN{};
	static std::vector<Pattern> POSSIBLE_PATTERNS
	{
		//Vertical pattenrs
		{{-2, 0}, {-1, 0}, {0, 0}},
		{{-1, 0}, {0, 0}, {1, 0}},
		{{0, 0}, {1, 0}, {2, 0}},
		//Horizontal patterns
		{{0, -2}, {0, -1}, {0, 0}},
		{{0, -2}, {0, -1}, {0, 0}},
		{{0, -2}, {0, -1}, {0, 0}},
		{{-1, 0}, {-1, 0}, {0, 0}, {1, 0}},
	};

	

	// Filenames for resources
	static std::string GREY_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_1.png");
	static std::string WHITE_TILE_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/tile_2.png");
	static std::string GRID_BACKGROUND_FILENAME("C:/Users/Levonog/Desktop/images/background.jpg");
	static std::string CONFIG_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/src/config.json");
	// Gems
	static std::string RED_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/red.png");
	static std::string ORANGE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/orange.png");
	static std::string GREEN_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/green.png");
	static std::string BLUE_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/blue.png");
	static std::string VIOLET_GEM_FILENAME("C:/Users/Levonog/Documents/GitHub/DevTestGame/resources/violet.png");

	// functions
	std::string getGemImageFilename(GemType gem_type);
	void printPattern(const Pattern& pattern);
	bool isValidPattern(const Pattern& pattern);
}

#endif // __UTILS_HPP__ 