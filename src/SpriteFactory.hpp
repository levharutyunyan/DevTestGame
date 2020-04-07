#ifndef __SPRITE_FACTORY_HPP__
#define __SPRITE_FACTORY_HPP__

#include "Tile.hpp"
#include <map>
#include <memory>
#include <vector>

#include "utils.hpp"
#include "Gem.hpp"
// #include "PowerUp.hpp"
//struct Gem;
//struct Tile;
// class PoweUp;

class SpriteFactory
{
private:
	using Gems = std::vector<Gem>;
	using Tiles = std::vector<Tile>;
	//using PowerUps = std::vector<PowerUp>;
public:
	SpriteFactory();

	Gem* getGem(utils::GemType gemType);
	//Tile* getTile(utils::TileType tileType);
	//PowerUp* getPowerUp(utils::PowerUpType poweUp);

private:
	void populateGems();
	void populateTiles();
	void populatePowerUps();
private:
	Gems _gems; 
	//Tiles _tiles;
	//PowerUps _powerUps;
};
#endif // __SPRITE_FACTORY_HPP__