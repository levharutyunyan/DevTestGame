#include "Piece.h"

Piece::Piece(int width = utils::GEM_WIDTH, int height = utils::GEM_HEIGHT)
	: _width(width)
	, _height(height)
	, _pieceType(utils::PieceType::NONE)
{
}
Piece::Piece()
	: Piece(utils::GEM_WIDTH, utils::GEM_HEIGHT)
{}

void Piece::setImage(utils::PieceType type)
{
	this->_pieceType = type;
	this->_filename = utils::getPieceImageFilename(this->_pieceType);
	this->_image.loadFromFile(this->_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}

//void Piece::setImage(const std::string & image_filename)
//{
//	this->_filename = image_filename;
//	this->_image.loadFromFile(image_filename);
//	this->_texture.loadFromImage(this->_image);
//	this->_sprite.setTexture(this->_texture);
//	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
//		static_cast<float>(this->_height) / this->_image.getSize().y);
//}

void Piece::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_sprite, states);
}

utils::PieceType Piece::getPieceType() const
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return utils::PieceType::RED; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return utils::PieceType::ORANGE; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return utils::PieceType::GREEN; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return utils::PieceType::BLUE; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return utils::PieceType::VIOLET; }
	if (this->_filename == utils::H_BOMB_FILENAME) { return utils::PieceType::H_BOMB; }
	if (this->_filename == utils::V_BOMB_FILENAME) { return utils::PieceType::V_BOMB; }
	if (this->_filename == utils::R_BOMB_FILENAME) { return utils::PieceType::R_BOMB; }
	return utils::PieceType::UNKNOWN;
}

void Piece::setPieceType(utils::PieceType type)
{
	this->_pieceType = type;
	setImage(type);
}

char Piece::getShortFilename() const
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return 'r'; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return 'o'; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return 'g'; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return 'b'; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return 'v'; }
	if (this->_filename == utils::NONE_GEM_FILENAME) { return 'n'; }
	if (this->_filename == utils::UNKNOWN_GEM_FILENAME) { return 'u'; }
}

void swap(Piece & lhs, Piece & rhs)
{
	utils::PieceType tmpType = lhs._pieceType;
	lhs.setPieceType(rhs._pieceType);
	rhs.setPieceType(tmpType);
}
