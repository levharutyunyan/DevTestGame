#ifndef __OBJECTIVES_HPP__
#define __OBJECTIVES_HPP__

// #include <map>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "utils.h"

class Objectives : public sf::Drawable, public sf::Transformable
{
public:
	Objectives();
	Objectives(const std::vector<std::pair<utils::PieceType, int>>& objectives);
	
	void updateObjectives(const std::vector<std::pair<utils::PieceType, int>>& objectives);
	bool areObjectivesLeft() const;
	std::vector<std::pair<utils::PieceType, int>> getObjectives() const;

private:
	std::vector<std::pair<utils::PieceType, int>> _objectives;
	std::vector<std::pair<Piece, sf::Text>> _gems;

	float _objectivesXPos;
	float _objectivesYPos;
	int _objectivesHeight;
	int _objectivesWidth;
	sf::RectangleShape _shape;
	sf::Color _shapeColor;
	sf::Font _textFont;
	sf::Color _fontColor;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};

#endif// __OBJECTIVES_HPP__