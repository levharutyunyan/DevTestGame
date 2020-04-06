#include "GameGrid.hpp"
#include <iostream>

GameGrid::GameGrid(const std::string& json_filename,
	std::vector<std::pair<int, utils::GemType>>* objectives)
	: _gridXPos(utils::GRID_Y_POS)
	, _gridYPos(utils::GRID_X_POS)
	, _rows(-1)
	, _columns(-1)
	, _figuresColorCount(-1)
	, _gridStatus(GridStatus::NONE)
	// , _gridXScale(static_cast<float>(utils::GRID_X_SCALE))
	// , _gridYScale(static_cast<float>(utils::GRID_Y_SCALE))
	, _objectives(objectives)
	, _gemPair(utils::NULL_GEM_PAIR)
{
	parseJsonFile(json_filename);
	getGraphicsElements();
	this->_affectedGems.resize(this->_gems.size(), std::vector<bool>(this->_gems[0].size()));
	this->_affectedColumns.resize(this->_gems[0].size(), true);
}

// GameGrid::GameGrid()
// 	: GameGrid(utils::CONFIG_FILENAME, utils::GRID_BACKGROUND_FILENAME, nullptr)
// {
// }

bool GameGrid::parseJsonFile(const std::string& json_filename)
{
	Json::Reader reader;
	Json::Value root;
	std::ifstream config_doc(utils::CONFIG_FILENAME);
	if (config_doc.is_open())
	{
		bool parseSuccess = reader.parse(config_doc, root, false);
		if (parseSuccess)
		{
			this->_columns = root["board_column_size"].asInt();
			this->_rows = root["board_row_size"].asInt();
			this->_figuresColorCount = root["figures_colors_count"].asInt();
		}
		else
		{
			std::cout << "parse is not successeful\n";
			return false;
		}
	}
	else
	{
		std::cout << "could not open document\n";
		return false;
	}
	return true;
}

void GameGrid::getGraphicsElements()
{
	// this->_bkgrImage.loadFromFile(bkgr_filename);
	// this->_gridXScale = static_cast<float>(utils::GRID_X_SCALE * utils::WINDOW_WIDTH) / this->_bkgrImage.getSize().x;
	// this->_gridYScale = static_cast<float>(utils::GRID_Y_SCALE * utils::WINDOW_HEIGHT) / this->_bkgrImage.getSize().y;

	// this->_bkgrTexture.loadFromImage(this->_bkgrImage);
	// this->_bkgrSprite.setTexture(this->_bkgrTexture);
	// this->_bkgrSprite.setPosition(this->_gridXPos, this->_gridYPos);
	// this->_bkgrSprite.setScale(static_cast<float>(this->_gridXScale),
	// 	static_cast<float>(this->_gridYScale));
	fillGems();
	fillTiles();
	// this->_affectedGems.resize(this->_gems.size(), std::vector<bool>(this->_gems[0].size()));
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
			this->_tiles[i][j].setPosition(static_cast<float>(this->_gridYPos + this->_tiles[i][j]._height * j),
				static_cast<float>(this->_gridXPos + this->_tiles[i][j]._width * i));
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
			this->_gems[i][j].setImage(Gem::randomGemFilename(this->_objectives, this->_figuresColorCount));
			this->_gems[i][j].setPosition(static_cast<float>(this->_gridYPos + utils::TILE_HEIGHT * j + 5),
				static_cast<float>(this->_gridXPos + utils::TILE_WIDTH * i + 5));
			// TODO: Change here later
			//std::cout << "grid_index: " << i << " " << j << "\n";
			//std::cout << "grid_index_filename: " << this->_gems[i][j]._filename << "\n";
		}
	}
}


void GameGrid::swapGems()
{
	 if (this->_gemPair.first == this->_gemPair.second || std::abs(this->_gemPair.first.x - this->_gemPair.second.x) + std::abs(this->_gemPair.first.y - this->_gemPair.second.y) > 1)
	 {
	 	this->_gridStatus = GridStatus::WAITING;
	 	return;
	 }
	std::cout << "swapping elements\n";
	std::cout << "left: " << this->_gemPair.first.x << " " << this->_gemPair.first.y << " right: " << this->_gemPair.second.x << " " << this->_gemPair.second.y << "\n";
	std::swap(this->_gems[this->_gemPair.first.x][this->_gemPair.first.y], this->_gems[this->_gemPair.second.x][this->_gemPair.second.y]);
	std::cout << "swapped elements\n";
	utils::Pattern lhsPosPattern = checkForMatch(this->_gemPair.first);
	utils::Pattern rhsPosPattern = checkForMatch(this->_gemPair.second);
	if(lhsPosPattern == utils::NULL_PATTERN && rhsPosPattern != utils::NULL_PATTERN)
	{
		std::swap(this->_gems[this->_gemPair.first.x][this->_gemPair.first.y], this->_gems[this->_gemPair.second.x][this->_gemPair.second.y]);
		this->_gridStatus = GridStatus::WAITING;
	}
	if (lhsPosPattern != utils::NULL_PATTERN)
	{
		popGems(lhsPosPattern);
	}
	if (rhsPosPattern != utils::NULL_PATTERN)
	{
		popGems(rhsPosPattern);
	}
	this->_gemPair = utils::NULL_GEM_PAIR;
}

sf::Vector2i GameGrid::getGridIndices(const sf::Vector2i & mouse_pos) const
{
	//std::cout << "gridXpos: " << this->_gridXPos << " mouseXPos " << mouse_pos.x << "\n";
	//std::cout << "gridYpos: " << this->_gridYPos << " mouseYPos " << mouse_pos.y << "\n";
	sf::Vector2i grid_index(mouse_pos.y - static_cast<int>(this->_gridYPos),
		mouse_pos.x - static_cast<int>(this->_gridYPos));

	grid_index.x = grid_index.x / this->_gems[0][0]._height;
	grid_index.y = grid_index.y / this->_gems[0][0]._width;
	// std::cout << "grid_index: " << grid_index.x << " " << grid_index.y << "\n";
	return grid_index;
}

void GameGrid::updateStatus()
{
	switch (this->_gridStatus)
	{
	case GridStatus::NONE:
	case GridStatus::CHECKING:
	{
		checkAffectedColumns();
		bool hasAffectedColumn = updateAffectedColumns();
		this->_gridStatus = (hasAffectedColumn ? GridStatus::DELETING : GridStatus::WAITING);
		break;
	}
	case GridStatus::DELETING:
	{
		deleteAffectedGems();

		this->_gridStatus = GridStatus::MOVING;
		break;
	}
	case GridStatus::MOVING:
	{
		dropAffectedColumns();

		this->_gridStatus = GridStatus::CREATING;
		break;
	}
	case GridStatus::CREATING:
	{
		generateNewGems();

		this->_gridStatus = GridStatus::CHECKING;
		break;
	}
	case GridStatus::SWAPPING:
	{
		swapGems();
		this->_gridStatus = GridStatus::DELETING;
		break;
	}
	case GridStatus::WAITING:
	{
		std::cout << "waiting\n";
		if(this->_gemPair != utils::NULL_GEM_PAIR)
		{
			this->_gridStatus = GridStatus::SWAPPING;
			std::cout << "waiting is over\n";
		}
		break;
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

	return (utils::isValidPattern(pattern) ? pattern : utils::Pattern());
}

void GameGrid::setGemPair(const utils::GemPair& gemPair)
{
	this->_gemPair = gemPair;
	this->_gridStatus = GridStatus::SWAPPING;
}

void GameGrid::setStatus(GridStatus status)
{
	this->_gridStatus = status;
}

void GameGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/*static int a = 0;
	if (a++ % 2 == 0)
		return;*/
	states.transform *= getTransform();

	for (int i = 0; i < this->_tiles.size(); ++i)
	{
		for (int j = 0; j < this->_tiles[i].size(); ++j)
		{
			target.draw(this->_tiles[i][j], states);
		}
	}
	//std::cout << "drawing over\n";
	for (int i = 0; i < this->_gems.size(); ++i)
	{
		for (int j = 0; j < this->_gems[i].size(); ++j)
		{
			if (!this->_affectedGems[i][j])
			{
				target.draw(this->_gems[i][j], states);
			}
			// HERE DRAW
			//std::cout << "drawing: " << "[ " << i << ": " << j << "] isDeleted: " << this->_affectedGems[i][j] << "\n";
		}
	}
	std::cout << "current grid situation_________\n";
	for (int i = 0; i < this->_affectedGems.size(); ++i)
	{
		for (int j = 0; j < this->_affectedGems[i].size(); ++j)
		{
			std::cout << "[" << this->_gems[i][j].getShortFilename() << " " << this->_affectedGems[i][j] << "], ";
		}
		std::cout << "\n";
	}
	std::cout << "_______________________________\n";
	system("pause");
	//std::cout << "drawing over\n";
}

// rename to searchByDirection
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

void GameGrid::popGems(const utils::Pattern & pattern)
{
	for (auto& pos : pattern)
	{
		this->_gems[pos.x][pos.y].pop();
		this->_affectedGems[pos.x][pos.y] = true;
		this->_affectedColumns[pos.y] = true;
	}
}

void GameGrid::checkColumn(int columnIndex)
{
	for(int i = 0; i < this->_gems.size(); ++i)
	{
		utils::Pattern tmpPattern = checkForMatch(sf::Vector2i(i, columnIndex));
		for(auto& it: tmpPattern)
		{
			this->_affectedGems[it.x][it.y] = true;
		}
	}
}

bool GameGrid::updateAffectedColumns()
{
	bool hasAffectedColumns = false;
	for(int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		if(this->_affectedColumns[i] == false)
		{
			continue;
		}
		bool isColumnAffected = false;
		for(int j = 0; j < this->_affectedGems.size(); ++j)
		{
			if(this->_affectedGems[j][i] == true)
			{
				isColumnAffected = true;
				hasAffectedColumns = true;
				break;
			}
		}
		this->_affectedColumns[i] = isColumnAffected;
	}
	return hasAffectedColumns;
}

void GameGrid::checkAffectedColumns()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{	
		/*if(this->_affectedColumns[i] == true)
		{*/
			checkColumn(i);
		//}
	}
}

void GameGrid::updateObjective(utils::GemType gemType)
{
	// auto it = this->_objectives.find(gemType);
	/*if(this->_objectives.find(gemType) != this->_objectives.end())
	{
		this->_objectives[gemType]--;
	}*/
}

void GameGrid::deleteAffectedGems()
{
	for (int j = 0; j < this->_affectedColumns.size(); ++j)
	{
		if (this->_affectedColumns[j] == true)
		{
			for(int i = 0; i < this->_affectedGems.size(); ++i)
			{
				if(this->_affectedGems[i][j] == true)
				{
					updateObjective(this->_gems[i][j].getGemType());				
					this->_gems[i][j].pop();
				}
			}
		}
	}
}

//void GameGrid::dropDown(int columnIndex)
//{
//	//Search first deleted gem
//	int deletedPos = this->_gems.size() - 1;
//	int notDeletedPos = -1;
//	while (notDeletedPos >= 0)
//	{
//		while (deletedPos > 0 && this->_affectedGems[deletedPos][columnIndex] == false)
//		{
//			--deletedPos;
//		}
//		//Search first not deleted gem
//		notDeletedPos = deletedPos;
//		while (notDeletedPos > 0 && this->_affectedGems[notDeletedPos][columnIndex] == true)
//		{
//			--notDeletedPos;
//		}
//		if (notDeletedPos == -1)
//		{
//			return;
//		}
//		std::swap(this->_gems[notDeletedPos][columnIndex], this->_gems[deletedPos][columnIndex]);
//		std::swap(this->_affectedGems[notDeletedPos][columnIndex], this->_affectedGems[deletedPos][columnIndex]);
//		/*std::cout << "swapping [deleted: not deleted]: " << deletedPos << ": " << notDeletedPos << "\n";
//		std::cout << "isDeleted [deleted: not deleted]: " << this->_gems[deletedPos][columnIndex]._isDeleted << ": " << this->_gems[notDeletedPos][columnIndex]._isDeleted << "\n";*/
//	}
//	/*std::cout << "dropDown column: " << columnIndex << "\n";
//	for (int i = 0; i < this->_gems.size(); ++i)
//	{
//		std::cout << this->_gems[i][columnIndex]._filename << " " << this->_affectedGems[i][columnIndex] << "\n";
//	}*/
//}


void GameGrid::dropDown(int columnIndex)
{
	for (int i = this->_affectedGems.size() - 2; i >= 0; --i)
	{
		if (this->_affectedGems[i][columnIndex] == false)
		{
			int j = i + 1;
			while (j < this->_affectedGems.size() && this->_affectedGems[j][columnIndex] == true)
			{
				swap(this->_gems[j - 1][columnIndex], this->_gems[j][columnIndex]);
				std::swap(this->_affectedGems[j - 1][columnIndex], this->_affectedGems[j][columnIndex]);
				/*std::string tmpFilename = this->_gems[j - 1][columnIndex]._filename;
				this->_gems[j - 1][columnIndex].setImage(this->_gems[j][columnIndex]._filename);
				this->_gems[j][columnIndex].setImage(tmpFilename);*/
				j++;
			}
			
		}
	}
}
//void GameGrid::rearangeGemsPositions()
//{
//	for (int i = 0; i < this->_gems.size(); ++i)
//	{
//		for (int j = 0; j < this->_gems[i].size(); ++j)
//		{
//			this->_gems[i][j].setPosition(static_cast<float>(this->_gridYPos + utils::TILE_HEIGHT * j + 5),
//				static_cast<float>(this->_gridXPos + utils::TILE_WIDTH * i + 5));
//		}
//	}
//}

void GameGrid::dropAffectedColumns()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		// if (this->_affectedColumns[i] == true)
		{
			dropDown(i);
		}
	}
}

void GameGrid::generateNewGems()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		// if (this->_affectedColumns[i] == true)
		{
			int initPos = 0;
			while (this->_affectedGems[initPos][i] == true)
			{
				this->_gems[initPos][i].setImage(Gem::randomGemFilename(this->_objectives, this->_figuresColorCount));
				//this->_gems[initPos][i]._isDeleted = false;
				this->_affectedGems[initPos][i] = false;
				++initPos;
			}
		}
	}
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
//
//GameGrid::Gem::Gem(int width = utils::GEM_WIDTH, int height = utils::GEM_HEIGHT)
//	: _width(width)
//	, _height(height)
//{
//}
//
//void GameGrid::Gem::setImage(const std::string & image_filename)
//{
//	this->_filename = image_filename;
//	this->_image.loadFromFile(image_filename);
//	this->_texture.loadFromImage(this->_image);
//	this->_sprite.setTexture(this->_texture);
//	this->_sprite.setScale(static_cast<float>(this->_width) / this->_image.getSize().x,
//		static_cast<float>(this->_height) / this->_image.getSize().y);
//}
//
//void GameGrid::Gem::draw(sf::RenderTarget & target, sf::RenderStates states) const
//{
//	states.transform *= getTransform();
//	target.draw(this->_sprite, states);
//}
//
//
//
//void GameGrid::Gem::pop()
//{
//}
//
//utils::GemType GameGrid::Gem::getGemType()
//{
//	if (this->_filename == utils::RED_GEM_FILENAME) { return utils::GemType::RED; }
//	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return utils::GemType::ORANGE; }
//	if (this->_filename == utils::GREEN_GEM_FILENAME) { return utils::GemType::GREEN; }
//	if (this->_filename == utils::BLUE_GEM_FILENAME) { return utils::GemType::BLUE; }
//	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return utils::GemType::VIOLET; }
//}
//
//std::string GameGrid::Gem::randomGemFilename(const std::vector<std::pair<int, utils::GemType>>* objs, int figures_colors_count)
//{
//	static std::vector<utils::GemType> gems;
//
//	// On initialization
//	if (gems.empty())
//	{
//		gems.resize(objs->size());
//		for (int i = 0; i < gems.size(); ++i)
//		{
//			gems[i] = (*objs)[i].second;
//		}
//		std::vector<utils::GemType> all_gem_types = utils::ALL_GEM_TYPES_VECTOR;
//		for (int i = 0; i < gems.size(); ++i)
//		{
//			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), gems[i]), all_gem_types.end());
//		}
//		for (std::size_t i = gems.size(); i < figures_colors_count; ++i)
//		{
//			utils::GemType tmp_gem = all_gem_types[rand() % all_gem_types.size()];
//			all_gem_types.erase(std::remove(all_gem_types.begin(), all_gem_types.end(), tmp_gem), all_gem_types.end());
//			gems.push_back(tmp_gem);
//		}
//	}
//
//	//Gem gem;
//	utils::GemType random_gem_type = gems[rand() % gems.size()];
//	return utils::getGemImageFilename(random_gem_type);
//}
//
//char GameGrid::Gem::getShortFilename() const
//{
//	if (this->_filename == utils::RED_GEM_FILENAME) { return 'r'; }
//	if (this->_filename == utils::ORANGE_GEM_FILENAME) { return 'o'; }
//	if (this->_filename == utils::GREEN_GEM_FILENAME) { return 'g'; }
//	if (this->_filename == utils::BLUE_GEM_FILENAME) { return 'b'; }
//	if (this->_filename == utils::VIOLET_GEM_FILENAME) { return 'v'; }
//}
//
//void swap(Gem & lhs, Gem & rhs)
//{
//	friend void swap(X& a, X& b)
//	{
//		using std::swap; // bring in swap for built-in types
//
//		swap(a.base1, b.base1);
//		swap(a.base2, b.base2);
//		// ...
//		swap(a.member1, b.member1);
//		swap(a.member2, b.member2);
//		// ...
//	}
//}
