#include "GameGrid.h"

GameGrid::GameGrid(const std::string& json_filename,
		const std::vector<std::pair<utils::PieceType, int>>& objectives)
	: _gridXPos(utils::GRID_Y_POS)
	, _gridYPos(utils::GRID_X_POS)
	, _rows(-1)
	, _columns(-1)
	, _figuresColorCount(-1)
	, _gridStatus(GridStatus::NONE)
	, _isSuccessfulTurn(false)
	, _objectives(objectives)
	//, _gemPair(utils::NULL_GEM_PAIR)
{
	parseJsonFile(json_filename);
	getGraphicsElements();
	this->_affectedGems.resize(this->_gems.size(), std::vector<bool>(this->_gems[0].size()));
	clearMarked();
	this->_affectedColumns.resize(this->_gems[0].size(), true);
	
	/*utils::PatternType findHorizontalPattern(sf::Vector2i position, 
		utils::Pattern& pattern, const std::vector<sf::Vector2i>& direction);*/
	/*this->_patternFinders.push_back(GameGrid::findHorizontalPattern);
	this->_patternFinders.push_back(GameGrid::findVerticalPattern);*/
	using namespace std::placeholders;
	this->_patternFinders.push_back(std::bind(&GameGrid::findHorizontalPattern, this, _1, _2));
	this->_patternFinders.push_back(std::bind(&GameGrid::findVerticalPattern, this, _1, _2));
	this->_patternFinders.push_back(std::bind(&GameGrid::findBoxlPattern, this, _1, _2));
	
	//this->_patternFinders.push_back(std::bind(GameGrid::findBoxlPattern, this, std::placeholders::_1))
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
				this->_tiles[i][j].setTileType(utils::TileType::WHITE_TILE);
			}
			else
			{
				this->_tiles[i][j].setTileType(utils::TileType::GREY_TILE);
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
			this->_gems[i][j].setImage(utils::randomGemType(this->_objectives, this->_figuresColorCount));
			this->_gems[i][j].setPosition(static_cast<float>(this->_gridYPos + utils::TILE_HEIGHT * j + 5),
				static_cast<float>(this->_gridXPos + utils::TILE_WIDTH * i + 5));
		
		}
	}
}

void GameGrid::clearMarked() const
{
	this->_markedGems = VectorBool(this->_rows, std::vector<bool>(this->_columns));
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
		
		setAffected(lhsPosPattern);
	}
	if (rhsPosPattern != utils::NULL_PATTERN)
	{
		printPattern(rhsPosPattern);
		setAffected(rhsPosPattern);
	}
	std::cout << "finished swapping\n";
	this->_gridStatus = GridStatus::DELETING;
	return true;
}

sf::Vector2i GameGrid::getGridIndices(const sf::Vector2i & mouse_pos) const
{
	//std::cout << "gridXpos: " << this->_gridXPos << " mouseXPos " << mouse_pos.x << "\n";
	//std::cout << "gridYpos: " << this->_gridYPos << " mouseYPos " << mouse_po s.y << "\n";
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
		noteAffectedGems();

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

void GameGrid::printPattern(const utils::Pattern & pattern) const
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
	//system("pause");
	//std::cout << "drawing over\n";
}

void GameGrid::searchByDirection(sf::Vector2i position, utils::Pattern & pattern, const sf::Vector2i & direction)
{
	while (0 <= position.x + direction.x && position.x + direction.x < this->_gems.size()
		&& 0 <= position.y + direction.y && position.y + direction.y < this->_gems[0].size()
		&& this->_gems[position.x][position.y].getPieceType() == this->_gems[position.x + direction.x][position.y + direction.y].getPieceType())
	{
		pattern.push_back(position + direction);
		position += direction;
	}
}

void GameGrid::setAffected(const utils::Pattern & pattern)
{
	for (auto& pos : pattern)
	{
		this->_affectedGems[pos.x][pos.y] = true;
		this->_affectedColumns[pos.y] = true;
	}
}

void GameGrid::detonateBomb(sf::Vector2i position)
{
	std::vector<sf::Vector2i> direction;
	switch (this->_gems[position.x][position.y].getPieceType())
	{
	case utils::PieceType::H_BOMB:
	{
		direction = utils::H_PATTERN_DIR;
		break;
	}
	case utils::PieceType::V_BOMB:
	{
		direction = utils::V_PATTERN_DIR;
		break;
	}
	case utils::PieceType::R_BOMB:
	{
		direction = utils::ALL_PATTERN_DIR;
		break;
	}
	}
	this->_gems[position.x][position.y].setPieceType(utils::PieceType::NONE);
	this->_affectedGems[position.x][position.y] = true;
	detonate(position, direction);
}

void GameGrid::detonate(sf::Vector2i position, const std::vector<sf::Vector2i>& direction)
{
	for (int i = 0; i < direction.size(); ++i)
	{
		sf::Vector2i currPos(position + direction[i]);
		while (0 <= currPos.x && currPos.x < this->_rows &&
			0 <= currPos.y && currPos.y < this->_columns)
		{
			utils::PieceType pieceType = this->_gems[currPos.x][currPos.y].getPieceType();
			if (utils::MAXIMUM_GEMS_COUNT < static_cast<int>(pieceType)
				&& static_cast<int>(pieceType) <= utils::MAXIMUM_BOMBS_COUNT + utils::MAXIMUM_GEMS_COUNT)
			{
				detonateBomb(currPos);
			}
			else
			{
				this->_affectedGems[currPos.x][currPos.y] = true;
			}
			currPos += direction[i];
		}
	}
}

utils::Pattern GameGrid::findPattern(sf::Vector2i position)
{
	////std::map<utils::Pattern, utils::PatternType> patterns;
	//utils::PatternType hPatternResult = findHorizontalPattern(position, hPattern);
	////patterns[hPattern] = hPatternResult;
	//utils::PatternType vPatternResult = findVerticalPattern(position, vPattern);
	////patterns[vPattern] = vPatternResult;
	//utils::PatternType bPatternResult = findBoxlPattern(position, bPattern);
	////patterns[bPattern] == bPatternResult;
	
	std::vector<std::pair<utils::Pattern, utils::PatternType>> patterns;
	for (int i = 0; i < this->_patternFinders.size(); ++i)
	{
		utils::Pattern pattern;
		utils::PatternType patternResult = this->_patternFinders[0](position, pattern);
		patterns.push_back({ pattern, patternResult });
	}

	utils::Pattern result;
	for (auto& it : patterns)
	{
		if (it.second != utils::PatternType::NONE && it.second != utils::PatternType::MATCH)
		{
			result = it.first;
			//turn in bomb here
			this->_gems[position.x][position.y].setPieceType(utils::toPieceType(it.second));
			this->_gems[position.x][position.y].setImage(this->_gems[position.x][position.y].getPieceType());
			result.erase(std::remove(result.begin(), result.end(), position), result.end());
			return result;
		}
	}
	for (auto & it : patterns)
	{
		if (it.second == utils::PatternType::MATCH)
		{
			return result;
		}
	}
	return utils::NULL_PATTERN;
}

utils::PatternType GameGrid::findHorizontalPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	collectPattern(position, pattern, utils::H_PATTERN_DIR);
	clearMarked();
	if (pattern.size() < 3) { return utils::PatternType::NONE; }
	return (pattern.size() > 3 ? utils::PatternType::H_BOMB : utils::PatternType::MATCH);
}

utils::PatternType GameGrid::findVerticalPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	collectPattern(position, pattern, utils::V_PATTERN_DIR);
	clearMarked();
	if (pattern.size() < 3) { return utils::PatternType::NONE; }
	return (pattern.size() > 3 ? utils::PatternType::V_BOMB : utils::PatternType::MATCH);
}

utils::PatternType GameGrid::findBoxlPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	const std::vector<sf::Vector2i>& direction = utils::ALL_PATTERN_DIR;
	this->_markedGems[position.x][position.y] = true;
	for (int i = 0; i < direction.size(); ++i)
	{
		sf::Vector2i newPosition(position.x + direction[i].x, position.y + direction[i].y);
		if (0 <= newPosition.x && newPosition.x < this->_rows &&
			0 <= newPosition.y && newPosition.y < this->_columns &&
			this->_gems[position.x][position.y].getPieceType() == this->_gems[newPosition.x][newPosition.y].getPieceType())
		{
			this->_markedGems[newPosition.x][newPosition.y] = true;
			pattern.push_back(newPosition);
		}
	}
	clearMarked();
	std::set<int> xCount;
	std::set<int> yCount;
	for (auto it : pattern)
	{
		xCount.insert(it.x);
		yCount.insert(it.y);
	}
	return (pattern.size() == 4 && xCount.size() == 2 && yCount.size() == 2 ? 
		utils::PatternType::R_BOMB : utils::PatternType::NONE);
}

void GameGrid::collectPattern(sf::Vector2i position, utils::Pattern & pattern, const std::vector<sf::Vector2i>& direction) const
{
	this->_markedGems[position.x][position.y] = true;
	for (int i = 0; i < direction.size(); ++i)
	{
		sf::Vector2i newPosition(position.x + direction[i].x, position.y + direction[i].y);
		if (0 <= newPosition.x && newPosition.x < this->_rows &&
			0 <= newPosition.y && newPosition.y < this->_columns &&
			this->_gems[position.x][position.y].getPieceType() == this->_gems[newPosition.x][newPosition.y].getPieceType())
		{
			pattern.push_back(newPosition);
			collectPattern(newPosition, pattern, direction);
		}
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

void GameGrid::noteAffectedGems()
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
				this->_gems[initPos][i].setImage(utils::randomGemType(this->_objectives, this->_figuresColorCount));
				this->_affectedGems[initPos][i] = false;
				++initPos;
			}
		}
	}
}

