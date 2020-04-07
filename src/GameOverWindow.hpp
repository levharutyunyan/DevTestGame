#ifndef __GAME_OVER_WINDOW_HPP__
#define __GAME_OVER_WINDOW_HPP__

#include <SFML/Graphics.hpp>
#include "utils.hpp"

class GameOverWindow : public sf::Drawable, public sf::Transformable
{
public:
	GameOverWindow();

	void haveWon(bool haveWon);
private:
	float _windowXPos;
	float _windowYPos;
	int _windowHeight;
	int _windowWidth;
	sf::RectangleShape _shape;
	sf::Color _shapeColor;
	sf::Text _message;
	sf::Font _textFont;
	sf::Color _fontColor;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif // __GAME_OVER_WINDOW_HPP__

