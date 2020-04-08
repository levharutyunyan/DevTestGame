#include "Turns.h"

Turns::Turns(const std::string& config_file)
	: _turnsXPos(utils::TURNS_X_POS)
	, _turnsYPos(utils::TURNS_Y_POS)
	, _turnsHeight(utils::TURNS_HEIGHT)
	, _turnsWidth(utils::TURNS_WIDTH)
	, _shape(sf::Vector2f(static_cast<float>(_turnsWidth), static_cast<float>(_turnsHeight)))
	, _shapeColor(utils::TURNS_COLOR)
	, _fontColor(utils::TURNS_FONT_COLOR)
{
	Json::Reader reader;
	Json::Value root;
	std::ifstream config_doc(utils::CONFIG_FILENAME);
	if (config_doc.is_open())
	{
		bool parseSuccess = reader.parse(config_doc, root, false);
		if (parseSuccess)
		{
			this->_turnsCount = root["moves_count"].asInt();
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	bool succeeded = this->_textFont.loadFromFile(utils::TURNS_FONT_FILENAME);
	if (!succeeded)
	{
		std::cout << "Can't load font\n";
		return;
	}
	this->_shape.setPosition(this->_turnsYPos, this->_turnsXPos);
	this->_shape.setFillColor(this->_shapeColor);

	this->_turns.setFont(this->_textFont);
	this->_turns.setCharacterSize(utils::TURNS_FONT_SIZE);
	this->_turns.setFillColor(this->_fontColor);
	this->_turns.setString(std::to_string(this->_turnsCount));
	//this->_turns.setPosition(this->_shape.getPosition().x + utils::TURNS_OFFSET.x, this->_shape.getPosition().y + utils::TURNS_OFFSET.y);
	this->_turns.setPosition(this->_shape.getPosition().x + this->_shape.getSize().x / 2  - utils::TURNS_OFFSET.x,
		this->_shape.getPosition().y + utils::TURNS_OFFSET.y);
}

void Turns::updateTurns()
{
	--this->_turnsCount;
	this->_turns.setString(std::to_string(this->_turnsCount));
}

bool Turns::areTurnsLeft() const
{
	return this->_turnsCount > 0;
}

void Turns::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_shape, states);
	target.draw(this->_turns, states);
}
