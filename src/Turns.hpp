#ifndef __TURNS_HPP__
#define __TURNS_HPP__

#include <fstream>
#include <SFML/Graphics.hpp>
#include "json/json.h"
#include "utils.hpp"

class Turns : public sf::Drawable, public sf::Transformable
{
public:
	Turns(const std::string& config_file);

	void updateTurns();
	bool areTurnsLeft() const;
private:

	float _turnsXPos;
	float _turnsYPos;
	int _turnsHeight;
	int _turnsWidth;
	sf::RectangleShape _shape;
	sf::Color _shapeColor;
	sf::Font _textFont;
	sf::Color _fontColor;
	sf::Text _turns;
	
	int _turnsCount;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // __TURNS_HPP__