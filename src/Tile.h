#ifndef __TILE_HPP__
#define __TILE_HPP__

#include <SFML/Graphics.hpp>
#include "utils.h"

struct Tile : public sf::Drawable, public sf::Transformable
{
	int _width;
	int _height;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;
	utils::TileType _tileType;
	Tile(int width, int height);
	Tile();
	
	utils::TileType getTileType() const;
	void setTileType(utils::TileType type);
private:
	void setImage(const std::string& image);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // __TILE_HPP__