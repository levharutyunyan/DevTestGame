#ifndef __GEM_HPP__
#define __GEM_HPP__

#include <SFML/Graphics.hpp>
#include "utils.hpp"

struct Gem : public sf::Drawable, public sf::Transformable
{
	int _width;
	int _height;
	std::string _filename;
	sf::Image _image;
	sf::Texture _texture;
	sf::Sprite _sprite;
	
	Gem(int width, int height);
	Gem();
	void setImage(const std::string& image_filename);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	friend void swap(Gem& lhs, Gem& rhs);
	void pop();
	utils::GemType getGemType();
	static std::string randomGemFilename(const std::vector<std::pair<int, utils::GemType>>* objs, int figures_colors_count);
	char getShortFilename() const;
};

#endif// __GEM_HPP__