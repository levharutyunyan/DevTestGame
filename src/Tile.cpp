#include "Tile.h"


Tile::Tile(int width = utils::TILE_WIDTH, int height = utils::TILE_HEIGHT)
	: _width(width)
	, _height(height)
{
}

Tile::Tile()
	: Tile(utils::TILE_WIDTH, utils::TILE_HEIGHT)
{}

utils::TileType Tile::getTileType() const
{
	return this->_tileType;
}
void Tile::setTexture(utils::TileType type)
{
	//this->_tileType = type;
	setImage(utils::getTileImageFilename(type));
}
void Tile::setTileType(utils::TileType type)
{
	this->_tileType = type;
}
void Tile::resetTexture()
{
	setTexture(this->_tileType);
}
void Tile::turnOnAffected()
{
	setTexture(utils::TileType::AFFECTED_TILE);
}
void Tile::toggleSelected()
{
	this->_isSelected = !this->_isSelected;
	if (this->_isSelected)
	{
		setTexture(utils::TileType::SELECTED_TILE);
	}
	else
	{
		reset();
	}
}
void Tile::reset()
{
	setTexture(this->_tileType);
}
int Tile::width() const
{
	return this->_width;
}
int Tile::height() const
{
	return this->_height;
}
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
