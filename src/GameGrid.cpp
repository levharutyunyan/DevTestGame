#include "GameGrid.hpp"
#include <iostream>

GameGrid::GameGrid(const std::string& json_filename,
		const std::vector<std::pair<utils::PieceType, int>>& objectives)
	: _gridXPos(utils::GRID_Y_POS)
	, _gridYPos(utils::GRID_X_POS)
	, _rows(-1)
	, _columns(-1)
	, _figuresColorCount(-1)
	, _gridStatus(GridStatus::NONE)
	, _isSuccessfulTurn(false)
	// , _gridXScale(static_cast<float>(utils::GRID_X_SCALE))
	// , _gridYScale(static_cast<float>(utils::GRID_Y_SCALE))
	, _objectives(objectives)
	//, _gemPair(utils::NULL_GEM_PAIR)
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
	this->_gems.resize(this->_rows, std::vector<Piece>(this->_columns));

	for (int i = 0; i < this->_gems.size(); ++i)
	{
		for (int j = 0; j < this->_gems[i].size(); ++j)
		{
			this->_gems[i][j].setImage(Piece::randomGemFilename(this->_objectives, this->_figuresColorCount));
			this->_gems[i][j].setPosition(static_cast<float>(this->_gridYPos + utils::TILE_HEIGHT * j + 5),
				static_cast<float>(this->_gridXPos + utils::TILE_WIDTH * i + 5));
		
		}
	}
}

bool GameGrid::swapGems(const utils::GemPair& gemPair)
{

	 if (gemPair.first == gemPair.second || std::abs(gemPair.first.x - gemPair.second.x) + std::abs(gemPair.first.y - gemPair.second.y) > 1)
	 {
		this->_isSuccessfulTurn = false;
	 	this->_gridStatus = GridStatus::WAITING;
	 	return false;
	 }
	std::cout << "swapping elements\n";
	std::cout << "left: " << gemPair.first.x << " " << gemPair.first.y << " right: " << gemPair.second.x << " " << gemPair.second.y << "\n";
	swap(this->_gems[gemPair.first.x][gemPair.first.y], this->_gems[gemPair.second.x][gemPair.second.y]);
	std::cout << "swapped elements\n";
	
	utils::Pattern lhsPosPattern = checkForMatch(gemPair.first);
	utils::Pattern rhsPosPattern = checkForMatch(gemPair.second);
	if(lhsPosPattern == utils::NULL_PATTERN && rhsPosPattern == utils::NULL_PATTERN)
	{
		this->_isSuccessfulTurn = false;
		swap(this->_gems[gemPair.first.x][gemPair.first.y], this->_gems[gemPair.second.x][gemPair.second.y]);
		this->_gridStatus = GridStatus::WAITING;
		return false;
	}
	if (lhsPosPattern != utils::NULL_PATTERN)
	{
		printPattern(lhsPosPattern);
		
		markGems(lhsPosPattern);
	}
	if (rhsPosPattern != utils::NULL_PATTERN)
	{
		printPattern(rhsPosPattern);
		markGems(rhsPosPattern);
	}
	std::cout << "finished swapping\n";
	this->_gridStatus = GridStatus::DELETING;
	return true;
}

sf::Vector2i GameGrid::getGridIndices(const sf::Vector2i & mouse_pos) const
{
	std::cout << "gridXpos: " << this->_gridXPos << " mouseXPos " << mouse_pos.x << "\n";
	std::cout << "gridYpos: " << this->_gridYPos << " mouseYPos " << mouse_pos.y << "\n";
	sf::Vector2i grid_index(mouse_pos.y - static_cast<int>(this->_gridXPos),
		mouse_pos.x - static_cast<int>(this->_gridYPos));

	grid_index.x = grid_index.x / this->_tiles[0][0]._height;
	grid_index.y = grid_index.y / this->_tiles[0][0]._width;
	 //std::cout << "grid_index: " << grid_index.x << " " << grid_index.y << "\n";
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
		//swapGems();
		this->_gridStatus = GridStatus::DELETING;
		break;
	}
	case GridStatus::WAITING:
	{
		//std::cout << "waiting\n";
		/*if(this->_gemPair != utils::NULL_GEM_PAIR)
		{
			this->_gridStatus = GridStatus::SWAPPING;
			std::cout << "waiting is over\n";
		}*/
		break;
	}
	}
}


utils::Pattern GameGrid::checkForMatch(const sf::Vector2i & position)
{
	utils::Pattern pattern;
	pattern.push_back(position);

	searchByDirection(position, pattern, sf::Vector2i(0, 1));
	searchByDirection(position, pattern, sf::Vector2i(0, -1));
	searchByDirection(position, pattern, sf::Vector2i(1, 0));
	searchByDirection(position, pattern, sf::Vector2i(-1, 0));

	return (utils::isValidPattern(pattern) ? pattern : utils::Pattern());
}

//void GameGrid::setGemPair(const utils::GemPair& gemPair)
//{
//	this->_gemPair = gemPair;
//	this->_gridStatus = GridStatus::SWAPPING;
//}

std::vector<std::pair<utils::PieceType, int>> GameGrid::getUpdatedObjectives() const
{
	return this->_objectives;
}

GridStatus GameGrid::getStatus() const
{
	return this->_gridStatus;
}

void GameGrid::setStatus(GridStatus status)
{
	this->_gridStatus = status;
}

void GameGrid::printPattern(const utils::Pattern & pattern)
{
	std::cout << "print pattern\n";
	for (int i = 0; i < pattern.size(); ++i)
	{
		std::cout << "[" << pattern[i].x << " ; " << pattern[i].y << "] { " << this->_gems[pattern[i].x][pattern[i].y].getShortFilename() << " }, ";
	}
	std::cout << "\n";

}

int GameGrid::getRows() const
{
	return this->_rows;
}

int GameGrid::getColumns() const
{
	return this->_columns;
}

bool GameGrid::getTurnResult() const
{
	return this->_isSuccessfulTurn;
}

void GameGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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
	//std::cout << "current grid situation_________\n";
	//for (int i = 0; i < this->_affectedGems.size(); ++i)
	//{
	//	for (int j = 0; j < this->_affectedGems[i].size(); ++j)
	//	{
	//		std::cout << "[" << this->_gems[i][j].getShortFilename() << " " << this->_affectedGems[i][j] << "], ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "_______________________________\n";
	//system("pause");
	//std::cout << "drawing over\n";
}

void GameGrid::searchByDirection(sf::Vector2i position, utils::Pattern & pattern, const sf::Vector2i & direction)
{
	while (0 <= position.x + direction.x && position.x + direction.x < this->_gems.size()
		&& 0 <= position.y + direction.y && position.y + direction.y < this->_gems[0].size()
		&& this->_gems[position.x][position.y]._filename == this->_gems[position.x + direction.x][position.y + direction.y]._filename)
	{
		pattern.push_back(position + direction);
		position += direction;
	}
}

void GameGrid::markGems(const utils::Pattern & pattern)
{
	for (auto& pos : pattern)
	{
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
			updateObjectives(this->_gems[it.x][it.y].getPieceType());
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

void GameGrid::updateObjectives(utils::PieceType PieceType)
{
	/*auto found = std::find_if(this->_objectives.begin(), this->_objectives.end(), 
	[&PieceType](auto a)
	{
		return (int)a.second == (int)type;
	});
	if (found != this->_objectives.end())
	{
		found->second--;
	}*/

	for (int i = 0; i < this->_objectives.size(); ++i)
	{
		if (this->_objectives[i].first == PieceType)
		{
			this->_objectives[i].second--;
			break;
		}
	}
}

void GameGrid::deleteAffectedGems()
{
	for (int j = 0; j < this->_affectedColumns.size(); ++j)
	{
		//if (this->_affectedColumns[j] == true)
		{
			for(int i = 0; i < this->_affectedGems.size(); ++i)
			{
				if(this->_affectedGems[i][j] == true)
				{
					updateObjectives(this->_gems[i][j].getPieceType());				
				}
			}
		}
	}
}

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
				this->_gems[initPos][i].setImage(Piece::randomGemFilename(this->_objectives, this->_figuresColorCount));
				this->_affectedGems[initPos][i] = false;
				++initPos;
			}
		}
	}
}

