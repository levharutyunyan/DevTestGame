#include "Spritefactory.hpp"


enum class GemType : int
{
	RED = 0, ORANGE = 1, GREEN = 2, BLUE = 3, VIOLET = 4
};
static const std::vector<GemType> ALL_GEM_TYPES_VECTOR
{ 
	GemType::RED, GemType::ORANGE, GemType::GREEN, GemType::BLUE, GemType::VIOLET 
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
	{
		
	}
}
