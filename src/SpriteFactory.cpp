#include "Spritefactory.hpp"

enum class PieceType : int
{
	RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4
};
static const std::vector<PieceType> ALL_GEM_TYPES_VECTOR
{ 
	PieceType::RED, PieceType::ORANGE, PieceType::GREEN, PieceType::BLUE, PieceType::VIOLET 
};

SpriteFactory::SpriteFactory()
{
	populateGems();
	// populateTiles();
	// populatePowerUps();
}

void SpriteFactory::populateGems()
{
	this->_gems.resize(utils::ALL_GEM_TYPES_VECTOR.size());
	
}
