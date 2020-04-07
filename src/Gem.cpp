#include "Gem.hpp"

Gem::Gem(int width = utils::GEM_WIDTH, int height = utils::GEM_HEIGHT)
	: _width(width)
	, _height(height)
{
}
Gem::Gem()
	: Gem(utils::GEM_WIDTH, utils::GEM_HEIGHT)
{}
void Gem::setImage(const std::string & image_filename)
{
	this->_filename = image_filename;
	this->_image.loadFromFile(image_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}

void Gem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_sprite, states);
}

utils::GemType Gem::getGemType()
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return utils::GemType::RED; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return utils::GemType::ORANGE; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return utils::GemType::GREEN; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return utils::GemType::BLUE; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return utils::GemType::VIOLET; }
	
	return utils::GemType::UNKNOWN;
}

std::string Gem::randomGemFilename(const std::vector<std::pair<utils::GemType, int>>& objs, int figures_colors_count)
{
	static std::vector<utils::GemType> gems;

	// On initialization
	if (gems.empty())
	{
		gems.resize(objs.size());
		for (int i = 0; i < gems.size(); ++i)
		{
			gems[i] = objs[i].first;
		}
		std::vector<utils::GemType> all_gem_types = utils::ALL_GEM_TYPES_VECTOR;
		for (int i = 0; i < gems.size(); ++i)
		{
			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), gems[i]), all_gem_types.end());
		}
		for (std::size_t i = gems.size(); i < figures_colors_count; ++i)
		{
			utils::GemType tmp_gem = all_gem_types[rand() % all_gem_types.size()];
			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), tmp_gem), all_gem_types.end());
			gems.push_back(tmp_gem);
		}
	}

	//Gem gem;
	utils::GemType random_gem_type = gems[rand() % gems.size()];
	return utils::getGemImageFilename(random_gem_type);
}

char Gem::getShortFilename() const
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return 'r'; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return 'o'; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return 'g'; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return 'b'; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return 'v'; }
}

void swap(Gem & lhs, Gem & rhs)
{
	using std::swap; 
	
	/*sf::Vector2f temp = lhs.getPosition();
	lhs.setPosition(rhs.getPosition());
	rhs.setPosition(temp);*/
	
	std::string tmpFilename = lhs._filename;
	lhs.setImage(rhs._filename);
	rhs.setImage(tmpFilename);
	
	swap(lhs._height, rhs._height);
	swap(lhs._width, rhs._width);
}
