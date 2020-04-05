#include "utils.hpp"

namespace utils
{
	std::string getGemImageFilename(GemType gem_type)
	{
		switch (gem_type)
		{
		case GemType::RED:
		{
			return RED_GEM_FILENAME;
		}
		case GemType::ORANGE:
		{
			return ORANGE_GEM_FILENAME;
		}
		case GemType::GREEN:
		{
			return GREEN_GEM_FILENAME;
		}
		case GemType::BLUE:
		{
			return BLUE_GEM_FILENAME;
		}
		case GemType::VIOLET:
		{
			return VIOLET_GEM_FILENAME;
		}
		}
	}
	void printPattern(const Pattern& pattern)
	{
		std::cout << "print pattern\n";
		for (int i = 0; i < pattern.size(); ++i)
		{
			std::cout << "[" << pattern[i].x << " ; " << pattern[i].y << "] ";
		}
		std::cout << "\n";
	}

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