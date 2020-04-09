#include "GameOverWindow.h"


GameOverWindow::GameOverWindow()
	: _windowXPos(utils::GAME_OVER_WINDOW_X_POS)
	, _windowYPos(utils::GAME_OVER_WINDOW_Y_POS)
	, _windowHeight(utils::GAME_OVER_WINDOW_HEIGHT)
	, _windowWidth(utils::GAME_OVER_WINDOW_WIDTH)
	, _shape(sf::Vector2f(static_cast<float>(_windowWidth), static_cast<float>(_windowHeight)))
	, _shapeColor(utils::GAME_OVER_COLOR)
	, _fontColor(utils::GAME_OVER_FONT_COLOR)	
{
	bool succeeded = this->_textFont.loadFromFile(utils::TURNS_FONT_FILENAME);
	if (!succeeded)
	{
		throw std::logic_error("Could not open font.\n");
	}

	this->_shape.setPosition(this->_windowXPos, this->_windowYPos);
	this->_shape.setFillColor(this->_shapeColor);

	this->_message.setFont(this->_textFont);
	this->_message.setCharacterSize(utils::TURNS_FONT_SIZE);
	this->_message.setFillColor(this->_fontColor);
	this->_message.setString("NULL");

	this->_message.setPosition(this->_shape.getPosition().x + utils::TURNS_OFFSET.x, 
		this->_shape.getPosition().y + this->_shape.getSize().y / 2 - utils::TURNS_OFFSET.y * 3);
}

void GameOverWindow::haveWon(bool haveWon)
{
	this->_message.setString((haveWon ? utils::GAME_OVER_WIN_TEXT : utils::GAME_OVER_LOSE_TEXT));
}

void GameOverWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_shape, states);
	target.draw(this->_message, states);
}