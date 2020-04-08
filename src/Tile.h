#ifndef __TILE_HPP__
#define __TILE_HPP__

#include <SFML/Graphics.hpp>
#include "utils.h"

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(int width, int height);
	Tile();
	
	utils::TileType getTileType() const;
	void setTexture(utils::TileType type);
	void setTileType(utils::TileType type);
	void resetTexture();
	void turnOnAffected();
	void toggleSelected();
	void reset();

	int width() const;
	int height() const;
private:
	void setImage(const std::string& image);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int _width;
	int _height;
	bool _isSelected;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;
	utils::TileType _tileType;
	
};

#endif // __TILE_HPP__