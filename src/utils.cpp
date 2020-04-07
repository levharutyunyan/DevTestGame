#include "utils.hpp"

namespace utils
{
	std::string getGemImageFilename(PieceType gem_type)
	{
		switch (gem_type)
		{
		case PieceType::RED:
		{
			return RED_GEM_FILENAME;
		}
		case PieceType::ORANGE:
		{
			return ORANGE_GEM_FILENAME;
		}
		case PieceType::GREEN:
		{
			return GREEN_GEM_FILENAME;
		}
		case PieceType::BLUE:
		{
			return BLUE_GEM_FILENAME;
		}
		case PieceType::VIOLET:
		{
			return VIOLET_GEM_FILENAME;
		}
		}
	}
	PieceType getPieceType(const std::string & gemName)
	{
		if (gemName == "red") { return PieceType::RED; }
		if (gemName == "orange") { return PieceType::ORANGE; }
		if (gemName == "green") { return PieceType::GREEN; }
		if (gemName == "blue") { return PieceType::BLUE; }
		if (gemName == "violet") { return PieceType::VIOLET; }
		return PieceType::UNKNOWN;
	}
	/*void printPattern(const Pattern& pattern)
	{
		std::cout << "print pattern\n";
		for (int i = 0; i < pattern.size(); ++i)
		{
			std::cout << "[" << pattern[i].x << " ; " << pattern[i].y << "] ";
		}
		std::cout << "\n";
	}*/

	bool isValidPattern(const Pattern & pattern)
	{
		if (pattern.size() < 3)
		{
			return false;
		}

		std::set<int> x_coords;
		std::set<int> y_coords;
		for (auto& it : pattern)
		{
			x_coords.insert(it.x);
			y_coords.insert(it.y);
		}

		return x_coords.size() >= 3 || y_coords.size() >= 3;
	}


}