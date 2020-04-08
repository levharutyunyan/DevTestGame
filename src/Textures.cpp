#include "Textures.h"


namespace utils
{
	std::map<PieceType, sf::Texture> Textures::_pieceTextures = std::map<PieceType, sf::Texture>();
	std::map<TileType, sf::Texture> Textures::_tileTextures = std::map<TileType, sf::Texture>();

	sf::Texture Textures::getPieceTexture(PieceType pieceType)
	{
		return _pieceTextures[pieceType];
	}
	sf::Texture Textures::getTileTexture(TileType tileType)
	{
		return _tileTextures[tileType];
	}
	sf::Texture Textures::getRandomGem(const std::vector<std::pair<utils::PieceType, int>>& objectives, int piecesCount)
	{
		static std::vector<utils::PieceType> pieces;

		// On initialization
		if (pieces.empty())
		{
			pieces.resize(objectives.size());
			for (int i = 0; i < pieces.size(); ++i)
			{
				pieces[i] = objectives[i].first;
			}
			std::vector<utils::PieceType> all_gem_types = utils::ALL_PIECE_TYPES_VECTOR;
			for (int i = 0; i < pieces.size(); ++i)
			{
				all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), pieces[i]), all_gem_types.end());
			}
			for (std::size_t i = pieces.size(); i < piecesCount; ++i)
			{
				utils::PieceType tmp_gem = all_gem_types[rand() % all_gem_types.size()];
				all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), tmp_gem), all_gem_types.end());
				pieces.push_back(tmp_gem);
			}
		}

		utils::PieceType random_gem_type = pieces[rand() % pieces.size()];
		return _pieceTextures[random_gem_type];

	}
	void Textures::populateTextures()
	{
		populatePieceTextures();
		populateTileTextures();
	}

	void Textures::populatePieceTextures()
	{
		for (int i = 0; i < ALL_PIECE_TYPES_VECTOR.size(); ++i)
		{
			sf::Texture tmp;
			_pieceTextures[ALL_PIECE_TYPES_VECTOR[i]] = tmp;
			tmp.loadFromFile(getPieceImageFilename(ALL_PIECE_TYPES_VECTOR[i]));
		}
	}
	
	void Textures::populateTileTextures()
	{
		for (int i = 0; i < ALL_TILE_TYPES_VECTOR.size(); ++i)
		{
			sf::Texture tmp;
			_tileTextures[ALL_TILE_TYPES_VECTOR[i]] = tmp;
			tmp.loadFromFile(getTileImageFilename(ALL_TILE_TYPES_VECTOR[i]));
		}
	}
}