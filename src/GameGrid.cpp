#include "GameGrid.hpp"
#include <iostream>


GameGrid::GameGrid(const std::string& json_filename, const sf::String& bkgr_filename,
	std::vector<std::pair<int, utils::GemType>>* objectives)
	: _gridXPos(utils::GRID_Y_POS)
	, _gridYPos(utils::GRID_X_POS)
	, _gridXScale(static_cast<float>(utils::GRID_X_SCALE))
	, _gridYScale(static_cast<float>(utils::GRID_Y_SCALE))
	, _objectives(objectives)
{
	parseJsonFile(json_filename);
	getGraphicsElements(bkgr_filename);
}


GameGrid::GameGrid()
	: GameGrid(utils::CONFIG_FILENAME, utils::GRID_BACKGROUND_FILENAME, nullptr)
{
}

bool GameGrid::isCorrectJsonFile(const std::string& json_filename,
	Json::Reader& reader, Json::Value& root) const
{
	std::ifstream config_doc(utils::CONFIG_FILENAME);
	if (config_doc.is_open())
	{
		bool parseSuccess = reader.parse(config_doc, root, false);
		if (parseSuccess)
		{
			if (!root["board_row_size"].isInt() || 7 > root["board_row_size"].asInt()
				|| root["board_row_size"].asInt() > 10)
			{
				return false;
			}
			if (!root["board_column_size"].isInt() || 7 > root["board_column_size"].asInt()
				|| root["board_column_size"].asInt() > 10)
			{
				return false;
			}
			if (!root["objectives"].isArray() || 1 > root["objectives"].size()
				|| root["objectives"].size() > 3)
			{
				return false;
			}
			if (!root["figures_colors_count"].isInt() || 3 > root["figures_colors_count"].asInt()
				|| root["figures_colors_count"].asInt() > 5)
			{
				return false;
			}

			//TODO if(undefined_color)
		}
		else
		{
			std::cout << reader.getFormatedErrorMessages();
			std::cout << reader.getFormattedErrorMessages();
			return false;
		}

	}
	return true;
}

bool GameGrid::parseJsonFile(const std::string& json_filename)
{
	Json::Reader reader;
	Json::Value root;
	if (!isCorrectJsonFile(json_filename, reader, root))
	{
		return false;
	}
	this->_columns = root["board_column_size"].asInt();
	this->_rows = root["board_row_size"].asInt();
	this->_figuresColorCount = root["figures_colors_count"].asInt();
	return true;
}

bool GameGrid::getGraphicsElements(const std::string& bkgr_filename)
{
	this->_bkgrImage.loadFromFile(bkgr_filename);
	this->_gridXScale = static_cast<float>(utils::GRID_X_SCALE * utils::WINDOW_WIDTH) / this->_bkgrImage.getSize().x;
	this->_gridYScale = static_cast<float>(utils::GRID_Y_SCALE * utils::WINDOW_HEIGHT) / this->_bkgrImage.getSize().y;

	this->_bkgrTexture.loadFromImage(this->_bkgrImage);
	this->_bkgrSprite.setTexture(this->_bkgrTexture);
	this->_bkgrSprite.setPosition(this->_gridXPos, this->_gridYPos);
	this->_bkgrSprite.setScale(static_cast<float>(this->_gridXScale),
		static_cast<float>(this->_gridYScale));
	fillGems();
	fillTiles();
	this->_affectedGems.resize(this->_gems.size(), std::vector<bool>(this->_gems[0].size()));
	return true;
}

void GameGrid::fillTiles()
{
	this->_tiles.resize(this->_rows, std::vector<Tile>(this->_columns));

	for (int i = 0; i < this->_tiles.size(); ++i)
	{
		for (int j = 0; j < this->_tiles[i].size(); ++j)
		{
			if ((i + j) % 2 == 0)
			{
				this->_tiles[i][j].setImage(utils::WHITE_TILE_FILENAME);
			}
			else
			{
				this->_tiles[i][j].setImage(utils::GREY_TILE_FILENAME);
			}
			this->_tiles[i][j].setPosition(static_cast<float>(this->_gridXPos + this->_tiles[i][j]._width * i),
				static_cast<float>(this->_gridYPos + this->_tiles[i][j]._width * j));
		}
	}
}

void GameGrid::fillGems()
{
	this->_gems.resize(this->_rows, std::vector<Gem>(this->_columns));

	for (int i = 0; i < this->_gems.size(); ++i)
	{
		for (int j = 0; j < this->_gems[i].size(); ++j)
		{
			this->_gems[i][j].setImage(GameGrid::Gem::randomGemFilename(this->_objectives, this->_figuresColorCount));
			this->_gems[i][j].setPosition(static_cast<float>(this->_gridXPos + this->_gems[i][j]._width * i),
				static_cast<float>(this->_gridYPos + this->_gems[i][j]._width * j));
		}
	}
}
//
//float GameGrid::gridXPos() const
//{
//	return this->_gridXPos;
//}
//
//float GameGrid::gridYpos() const
//{
//	return this->_gridYPos;
//}

void GameGrid::swapGems(const sf::Vector2i& lhsPos, const sf::Vector2i& rhsPos)
{
	if (lhsPos == rhsPos || std::abs(lhsPos.x - rhsPos.x) > 1 || std::abs(lhsPos.y - rhsPos.y) > 1)
	{
		return;
	}
	std::cout << "swapping elements\n";
	std::cout << "left: " << lhsPos.x << " " << lhsPos.y << " right: " << rhsPos.x << " " << rhsPos.y << "\n";
	std::swap(this->_gems[lhsPos.x][lhsPos.y], this->_gems[rhsPos.x][rhsPos.y]);
	std::cout << "swapped elements\n";
	utils::Pattern lhsPosPattern = checkForMatch(lhsPos);
	utils::Pattern rhsPosPattern = checkForMatch(rhsPos);
	bool islhsPosPatternValid = utils::isValidPattern(lhsPosPattern);
	bool isrhsPosPatternValid = utils::isValidPattern(rhsPosPattern);
	if (islhsPosPatternValid == false && isrhsPosPatternValid == false)
	{
		std::swap(this->_gems[lhsPos.x][lhsPos.y], this->_gems[rhsPos.x][rhsPos.y]);
	}
	else if (islhsPosPatternValid == false)
	{
		popGems(rhsPosPattern);
	}
	else if (isrhsPosPatternValid == false)
	{
		popGems(lhsPosPattern);
	}
	else
	{
		popGems(lhsPosPattern);
		popGems(rhsPosPattern);
	}

}

sf::Vector2i GameGrid::getGridIndices(const sf::Vector2i & mouse_pos) const
{
	//std::cout << "gridXpos: " << this->_gridXPos << " mouseXPos " << mouse_pos.x << "\n";
	//std::cout << "gridYpos: " << this->_gridYPos << " mouseYPos " << mouse_pos.y << "\n";
	sf::Vector2i grid_index(mouse_pos.x - static_cast<int>(this->_gridXPos),
		mouse_pos.y - static_cast<int>(this->_gridYPos));

	grid_index.x = grid_index.x / this->_gems[0][0]._width;
	grid_index.y = grid_index.y / this->_gems[0][0]._height;
	std::cout << "grid_index: " << grid_index.x << " " << grid_index.y << "\n";
	return grid_index;
}

void GameGrid::updateGrid()
{
	switch (this->_gridStatus)
	{
	case GridStatus::NONE:
	{
	}
	case GridStatus::CREATING:
	{
	}
	case GridStatus::CHECKING:
	{
	}
	case GridStatus::MOVING:
	{
	}
	case GridStatus::DELETING:
	{
	}
	}
}

utils::Pattern GameGrid::checkForMatch(const sf::Vector2i & position)
{
	utils::Pattern pattern;
	pattern.push_back(position);

	findPattern(position, pattern, sf::Vector2i(0, 1));
	findPattern(position, pattern, sf::Vector2i(0, -1));
	findPattern(position, pattern, sf::Vector2i(1, 0));
	findPattern(position, pattern, sf::Vector2i(-1, 0));


	return (pattern.size() >= 3 ? pattern : utils::Pattern());
}

void GameGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();
	target.draw(this->_bkgrSprite, states);

	for (int i = 0; i < this->_tiles.size(); ++i)
	{
		for (int j = 0; j < this->_tiles[i].size(); ++j)
		{
			target.draw(this->_tiles[i][j], states);
		}
	}
	for (int i = 0; i < this->_gems.size(); ++i)
	{
		for (int j = 0; j < this->_gems[i].size(); ++j)
		{
			target.draw(this->_gems[i][j], states);
		}
	}

}

void GameGrid::findPattern(sf::Vector2i position, utils::Pattern & pattern, const sf::Vector2i & direction)
{
	while (0 <= position.x + direction.x && position.x + direction.x < this->_gems.size()
		&& 0 <= position.y + direction.y && position.y + direction.y < this->_gems[0].size()
		&& this->_gems[position.x][position.y]._filename == this->_gems[position.x + direction.x][position.y + direction.y]._filename)
	{
		pattern.push_back(position + direction);
		position += direction;
	}
}

bool GameGrid::matchPattern(const utils::Pattern & pattern)
{
	/*if (matches some particular pattern)
	{
		add new power up;
	}*/
	return pattern.size() > 2;
}

void GameGrid::popGems(const utils::Pattern & pattern)
{
	for (auto& pos : pattern)
	{
		this->_gems[pos.x][pos.y].pop();
		this->_affectedGems[pos.x][pos.y] = true;
		this->_affectedColumns.push_back(pos.y);
	}
}

void GameGrid::checkColumn(int columnIndex)
{
	bool areNoMatches = true;
	while (areNoMatches)
	{
		for (int i = 0; i < this->_affectedColumns.size(); ++i)
		{

		}
	}
}

void GameGrid::dropDown(int columnIndex)
{
	int initPos = 0;
	while (this->_gems[columnIndex][initPos + 1]._isDeleted == false && initPos + 1 != this->_rows)
	{
		++initPos;
	}
	int depth = 0;
	while (this->_gems[columnIndex][initPos + depth + 1]._isDeleted == false && initPos + depth + 1 != this->_rows)
	{
		++depth;
	}
	while (initPos >= 0)
	{
		int tmpPos = initPos;
		for (int i = 0; i < depth; ++i)
		{
			std::swap(sf::Vector2i(columnIndex, tmpPos), sf::Vector2i(columnIndex, tmpPos + 1));
		}
		++initPos;
	}
	for (int i = 0; i < depth; ++i)
	{
		this->_gems[columnIndex][i].setImage(GameGrid::Gem::randomGemFilename(this->_objectives, this->_figuresColorCount));
	}
}

void GameGrid::dropColumns()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		dropDown(this->_affectedColumns[i]);
	}
	this->_affectedColumns.clear();
}

GameGrid::Tile::Tile(int width = utils::TILE_WIDTH, int height = utils::TILE_HEIGHT)
	: _width(width)
	, _height(height)
{
}
void GameGrid::Tile::setImage(const std::string& image_filename)
{
	this->_image.loadFromFile(image_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}
void GameGrid::Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(this->_sprite, states);
}

GameGrid::Gem::Gem(int width = utils::TILE_WIDTH, int height = utils::TILE_HEIGHT)
	: _width(width)
	, _height(height)
	, _isDeleted(false)
{
}

void GameGrid::Gem::setImage(const std::string & image_filename)
{
	this->_filename = image_filename;
	this->_image.loadFromFile(image_filename);
	this->_texture.loadFromImage(this->_image);
	this->_sprite.setTexture(this->_texture);
	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
		static_cast<float>(this->_height) / this->_image.getSize().y);
}

void GameGrid::Gem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (this->_isDeleted == false)
	{
		states.transform *= getTransform();
		target.draw(this->_sprite, states);
	}
}

void GameGrid::Gem::pop()
{
	this->_isDeleted = true;
}

std::string GameGrid::Gem::randomGemFilename(const std::vector<std::pair<int, utils::GemType>>* objs, int figures_colors_count)
{
	static std::vector<utils::GemType> gems;

	// On initialization
	if (gems.empty())
	{
		gems.resize(objs->size());
		for (int i = 0; i < gems.size(); ++i)
		{
			gems[i] = (*objs)[i].second;
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
