#include "Piece.hpp"

Piece::Piece(int width = utils::GEM_WIDTH, int height = utils::GEM_HEIGHT)
	: _width(width)
	, _height(height)
{
}
Piece::Piece()
	: Piece(utils::GEM_WIDTH, utils::GEM_HEIGHT)
{}
void Piece::setImage(const std::string & image_filename)
{
	this->_filename = image_filename;
	this->_image.loadFromFile(image_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}

void Piece::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_sprite, states);
}

utils::PieceType Piece::getPieceType()
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return utils::PieceType::RED; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return utils::PieceType::ORANGE; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return utils::PieceType::GREEN; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return utils::PieceType::BLUE; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return utils::PieceType::VIOLET; }
	
	return utils::PieceType::UNKNOWN;
}

std::string Piece::randomGemFilename(const std::vector<std::pair<utils::PieceType, int>>& objs, int figures_colors_count)
{
	static std::vector<utils::PieceType> gems;

	// On initialization
	if (gems.empty())
	{
		gems.resize(objs.size());
		for (int i = 0; i < gems.size(); ++i)
		{
			gems[i] = objs[i].first;
		}
		std::vector<utils::PieceType> all_gem_types = utils::ALL_GEM_TYPES_VECTOR;
		for (int i = 0; i < gems.size(); ++i)
		{
			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), gems[i]), all_gem_types.end());
		}
		for (std::size_t i = gems.size(); i < figures_colors_count; ++i)
		{
			utils::PieceType tmp_gem = all_gem_types[rand() % all_gem_types.size()];
			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), tmp_gem), all_gem_types.end());
			gems.push_back(tmp_gem);
		}
	}

	//Gem gem;
	utils::PieceType random_gem_type = gems[rand() % gems.size()];
	return utils::getGemImageFilename(random_gem_type);
}

char Piece::getShortFilename() const
{
	if (this->_filename == utils::RED_GEM_FILENAME) { return 'r'; }
	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return 'o'; }
	if (this->_filename == utils::GREEN_GEM_FILENAME) { return 'g'; }
	if (this->_filename == utils::BLUE_GEM_FILENAME) { return 'b'; }
	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return 'v'; }
}

void swap(Piece & lhs, Piece & rhs)
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
