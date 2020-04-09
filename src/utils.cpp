#include "utils.h"

namespace utils
{
	PieceType randomGemType(const std::vector<std::pair<PieceType, int>>& objs, int figures_colors_count)
	{
		static std::vector<PieceType> gems;

		// On initialization
		if (gems.empty())
		{
			gems.resize(objs.size());
			for (int i = 0; i < gems.size(); ++i)
			{
				gems[i] = objs[i].first;
			}
			std::vector<PieceType> all_gem_types = ALL_PIECE_TYPES_VECTOR;
			auto found = std::find_if(all_gem_types.begin(), all_gem_types.end(),
				[](auto& a) -> bool {return a >= PieceType::H_BOMB; });
			auto pos = std::distance(all_gem_types.begin(), found);

			all_gem_types.resize(pos);
			for (int i = 0; i < gems.size(); ++i)
			{
				all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), gems[i]), all_gem_types.end());
			}
			for (std::size_t i = gems.size(); i < figures_colors_count; ++i)
			{
				PieceType tmp_gem = all_gem_types[rand() % all_gem_types.size()];
				all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), tmp_gem), all_gem_types.end());
				gems.push_back(tmp_gem);
			}
		}

		//Gem gem;
		PieceType random_gem_type = gems[rand() % gems.size()];
		return random_gem_type;
	}
	std::string getPieceImageFilename(PieceType gemType)
	{
		switch (gemType)
		{
		case PieceType::NONE:
		{
			return NONE_GEM_FILENAME;
		}
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
		case PieceType::H_BOMB:
		{
			return H_BOMB_FILENAME;
		}
		case PieceType::V_BOMB:
		{
			return V_BOMB_FILENAME;
		}
		case PieceType::R_BOMB:
		{
			return R_BOMB_FILENAME;
		}
		default: 
		{
			return UNKNOWN_GEM_FILENAME;
		}
		}
	}
	PieceType toPieceType(PatternType patternType)
	{
		switch (patternType)
		{
		case PatternType::H_BOMB:
		{
			return PieceType::H_BOMB;
		}
		case PatternType::V_BOMB:
		{
			return PieceType::V_BOMB;
		}
		case PatternType::R_BOMB:
		{
			return PieceType::R_BOMB;
		}
		}
	}
	std::string getTileImageFilename(TileType tileType)
	{
		switch (tileType)
		{
		case TileType::GREY_TILE:
		{
			return GREY_TILE_FILENAME;
		}
		case TileType::WHITE_TILE:
		{
			return WHITE_TILE_FILENAME;
		}
		case TileType::AFFECTED_TILE:
		{
			return AFFECTED_TILE_FILENAME;
		}
		case TileType::SELECTED_TILE:
		{
			return SELECTED_TILE_FILENAME;
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

	bool compareVector2i::operator()(const sf::Vector2i & lhs, const sf::Vector2i & rhs)
	{
		if ((lhs.x + lhs.y)*(lhs.x + lhs.y) < (rhs.x + rhs.y)*(rhs.x + rhs.y))
		{
			return false;
		}
		if (lhs.x < rhs.x)
		{
			return false;
		}
		return lhs.y < rhs.y;
	}
}