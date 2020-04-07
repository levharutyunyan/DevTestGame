#include "Objectives.hpp"

	//float _objectivesXPos;
	//float _objectivesYPos;
	//int _objectivesHeight;
	//int _objectivesWidth;
	//sf::RectangleShape _shape;
	//sf::Color _color

Objectives::Objectives()
{}

Objectives::Objectives(const std::vector<std::pair<utils::PieceType, int>>& objectives)
	: _objectives(objectives)
	, _objectivesXPos(utils::OBJECTIVES_X_POS)
	, _objectivesYPos(utils::OBJECTIVES_Y_POS)
	, _objectivesHeight(utils::OBJECTIVES_HEIGHT)
	, _objectivesWidth(utils::OBJECTIVES_WIDTH)
	, _shape(sf::Vector2f(static_cast<float>(_objectivesWidth), static_cast<float>(_objectivesHeight)))
	, _shapeColor(utils::OBJECTIVES_COLOR)
	, _fontColor(utils::OBJECTIVES_FONT_COLOR)

{
	this->_shape.setPosition(this->_objectivesXPos, this->_objectivesYPos);
	this->_shape.setFillColor(this->_shapeColor);
	
	bool succeeded = this->_textFont.loadFromFile(utils::OBJECTIVES_FONT_FILENAME);
	if (!succeeded)
	{
		std::cout << "Can't load font\n";
		return;
	}
	
	this->_gems.resize(this->_objectives.size());

	for(int i = 0; i < this->_objectives.size(); ++i)
	{
		// std::string getGemImageFilename(PieceType gem_type);
		this->_gems[i].first.setImage(getGemImageFilename(this->_objectives[i].first));		
		this->_gems[i].first.setPosition(this->_shape.getPosition().x + utils::OBJECTIVES_OFFSET.y + utils::OBJECTIVES_OFFSET.x * i, this->_shape.getPosition().y + utils::OBJECTIVES_OFFSET.y);
		this->_gems[i].second.setFont(this->_textFont);
		this->_gems[i].second.setCharacterSize(utils::OBJECTIVES_FONT_SIZE);
		this->_gems[i].second.setFillColor(this->_fontColor);
		this->_gems[i].second.setString(std::to_string(this->_objectives[i].second));		
		this->_gems[i].second.setPosition(this->_shape.getPosition().x + utils::OBJECTIVES_OFFSET.y  + utils::OBJECTIVES_OFFSET.x * i, this->_shape.getPosition().x + utils::OBJECTIVES_OFFSET.x + utils::OBJECTIVES_OFFSET.y);
	}	
}

void Objectives::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_shape, states);

	for(int i = 0; i < this->_gems.size(); ++i)
	{
		if (this->_objectives[i].second > 0)
		{
			target.draw(this->_gems[i].first, states);		
			target.draw(this->_gems[i].second, states);
		}
	}
}
// std::vector<std::pair<utils::PieceType, int>> _objectives;
// std::vector<std::pair<Gem, sf::Text>> _gems;

void Objectives::updateObjectives(const std::vector<std::pair<utils::PieceType, int>>& objectives)
{
	//system("pause");
	this->_objectives = objectives;
	for (int i = 0; i < this->_objectives.size(); ++i)
	{
		this->_gems[i].second.setString(std::to_string(this->_objectives[i].second));
	}
}

bool Objectives::areObjectivesLeft() const
{
	for (int i = 0; i < this->_objectives.size(); ++i)
	{
		if (this->_objectives[i].second > 0)
		{
			return true;
		}
	}
	return false;
}

std::vector<std::pair<utils::PieceType, int>> Objectives::getObjectives() const
{
	return this->_objectives;
}

