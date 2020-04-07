#ifndef __PIECE_HPP__
#define __PIECE_HPP__

#include <SFML/Graphics.hpp>
#include "utils.hpp"

struct Piece : public sf::Drawable, public sf::Transformable
{
	int _width;
	int _height;
	std::string _filename;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;
	
	Piece(int width, int height);
	Piece();
	void setImage(const std::string& image_filename);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	friend void swap(Piece& lhs, Piece& rhs);
	utils::PieceType getPieceType();
	static std::string randomGemFilename(const std::vector<std::pair<utils::PieceType, int>>& objs, int figures_colors_count);
	char getShortFilename() const;
};

#endif// __PIECE_HPP__