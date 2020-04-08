#ifndef __PIECE_HPP__
#define __PIECE_HPP__

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "utils.h"

struct Piece : public sf::Drawable, public sf::Transformable
{

public:
	Piece(int width, int height);
	Piece();
	//void setImage(const std::string& image_filename);
	void setImage(utils::PieceType type);
	friend void swap(Piece& lhs, Piece& rhs);
	utils::PieceType getPieceType() const;
	void setPieceType(utils::PieceType type);
	char getShortFilename() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	int _width;
	int _height;
	utils::PieceType _pieceType;
	std::string _filename;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;
};

#endif// __PIECE_HPP__