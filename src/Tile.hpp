#ifndef __TILE_HPP__
#define __TILE_HPP__

#include <SFML/Graphics.hpp>
#include "utils.hpp"

struct Tile : public sf::Drawable, public sf::Transformable
{
	int _width;
	int _height;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;

	Tile(int width, int height);
	Tile();
	
	void setImage(const std::string& image);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // __TILE_HPP__