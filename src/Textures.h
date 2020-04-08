#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "Piece.h"
#include "Tile.h"

enum class PieceType : int
{
	RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4, UNKNOWN = 5,
	V_BOMB = 100, H_BOMB = 101, R_BOMB = 102,
};

enum class TileType : int
{
	GreyTile = 0, WhiteTile = 1
};

namespace utils
{

class Textures
{
private:
	Textures(const Textures& other) = delete;
	Textures operator=(const Textures& other) = delete;
	Textures() = delete;

public:
	sf::Texture getPieceTexture(PieceType pieceType);
	sf::Texture getTileTexture(TileType tileType);
	sf::Texture getRandomGem(const std::vector<std::pair<utils::PieceType, int>>& objectives, int piecesCount);

	static void populateTextures();
private:
	static void populatePieceTextures();
	static void populateTileTextures();

private:
	static std::map<PieceType, sf::Texture> _pieceTextures;
	static std::map<TileType, sf::Texture> _tileTextures;
};

}

#endif // __TEXTURES_H__