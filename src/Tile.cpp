#include "Tile.hpp"


Tile::Tile(int width = utils::TILE_WIDTH, int height = utils::TILE_HEIGHT)
	: _width(width)
	, _height(height)
{
}

Tile::Tile()
	: Tile(utils::TILE_WIDTH, utils::TILE_HEIGHT)
{}
void Tile::setImage(const std::string& image_filename)
{
	this->_image.loadFromFile(image_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}
void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_sprite, states);
}
