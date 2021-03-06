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
{
	this->_secondsPassed = this->_clock.getElapsedTime().asSeconds();
	
	parseJsonFile(json_filename);
	getGraphicsElements();
	this->_affectedGems.resize(this->_gems.size(), std::vector<bool>(this->_gems[0].size()));
	clearMarked();
	this->_affectedColumns.resize(this->_gems[0].size(), true);
	
	using namespace std::placeholders;
	this->_patternFinders.push_back(std::bind(&GameGrid::findBoxPattern, this, _1, _2));
	this->_patternFinders.push_back(std::bind(&GameGrid::findHorizontalPattern, this, _1, _2));
	this->_patternFinders.push_back(std::bind(&GameGrid::findVerticalPattern, this, _1, _2));
}

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
			throw std::logic_error(reader.getFormattedErrorMessages());
		}
	}
	else
	{
		throw std::logic_error("Could not open connfig file.\n");
	}
	return true;
}

void GameGrid::getGraphicsElements()
{
	fillGems();
	fillTiles();
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
				this->_tiles[i][j].setTexture(utils::TileType::WHITE_TILE);
				this->_tiles[i][j].setTileType(utils::TileType::WHITE_TILE);
			}
			else
			{
				this->_tiles[i][j].setTexture(utils::TileType::GREY_TILE);
				this->_tiles[i][j].setTileType(utils::TileType::GREY_TILE);
			}
			this->_tiles[i][j].setPosition(static_cast<float>(this->_gridYPos + this->_tiles[i][j].height() * j),
				static_cast<float>(this->_gridXPos + this->_tiles[i][j].width() * i));
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
	
	utils::Pattern lhsPosPattern = findPattern(gemPair.first);
	utils::Pattern rhsPosPattern = findPattern(gemPair.second);
	if(lhsPosPattern == utils::NULL_PATTERN && rhsPosPattern == utils::NULL_PATTERN)
	{
		this->_isSuccessfulTurn = false;
		swap(this->_gems[gemPair.first.x][gemPair.first.y], this->_gems[gemPair.second.x][gemPair.second.y]);
		this->_gridStatus = GridStatus::WAITING;
		return false;
	}
	if (lhsPosPattern != utils::NULL_PATTERN)
	{
		//printPattern(lhsPosPattern);
		
		setAffected(lhsPosPattern);
	}
	if (rhsPosPattern != utils::NULL_PATTERN)
	{
		//printPattern(rhsPosPattern);
		setAffected(rhsPosPattern);
	}
	std::cout << "finished swapping\n";
	this->_gridStatus = GridStatus::DELETING;
	return true;
}

sf::Vector2i GameGrid::getGridIndices(const sf::Vector2i & mouse_pos) const
{
	sf::Vector2i grid_index(mouse_pos.y - static_cast<int>(this->_gridXPos),
		mouse_pos.x - static_cast<int>(this->_gridYPos));

	grid_index.x = grid_index.x / this->_tiles[0][0].height();
	grid_index.y = grid_index.y / this->_tiles[0][0].width();
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
		this->_gridStatus = GridStatus::DELETING;
		break;
	}
	case GridStatus::WAITING:
	{
		this->_secondsPassed = this->_clock.getElapsedTime().asSeconds();
		if (this->_secondsPassed > 4)
		{
			std::cout << _secondsPassed << "\n";
			_secondsPassed -= 4;
			this->_clock.restart();
			utils::Pattern pat;
			bool possibleMatch = findPossibleMatch(pat);
			if (possibleMatch != true)
			{
				std::cout << "shuffling\n";
				shuffle();
				this->_gridStatus = GridStatus::CHECKING;
			}
		}
		break;
	}
	}
}

std::vector<std::pair<utils::PieceType, int>> GameGrid::getUpdatedObjectives() const
{
	return this->_objectives;
}

void GameGrid::toggleTile(sf::Vector2i pos)
{
	std::cout << "toggling " << pos.x << " " << pos.y << "\n";
	this->_tiles[pos.x][pos.y].toggleSelected();
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

bool GameGrid::isBomb(sf::Vector2i pos) const
{
	return static_cast<int>(utils::MINIMUM_BOMBS_COUNT) < static_cast<int>(this->_gems[pos.x][pos.y].getPieceType()) &&
		static_cast<int>(this->_gems[pos.x][pos.y].getPieceType()) < static_cast<int>(utils::MAXIMUM_BOMBS_COUNT);
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
	for (int i = 0; i < this->_gems.size(); ++i)
	{
		for (int j = 0; j < this->_gems[i].size(); ++j)
		{
			if (!this->_affectedGems[i][j])
			{
				target.draw(this->_gems[i][j], states);
			}
		}
	}
}


void GameGrid::setAffected(const utils::Pattern & pattern)
{
	for (auto& pos : pattern)
	{
		this->_affectedGems[pos.x][pos.y] = true;
		this->_tiles[pos.x][pos.y].turnOnAffected();
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
	this->_affectedColumns[position.y] = true;
	this->_tiles[position.x][position.y].turnOnAffected();
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
				this->_affectedColumns[currPos.y] = true;
				this->_affectedGems[currPos.x][currPos.y] = true;
				this->_tiles[currPos.x][currPos.y].turnOnAffected();
			}
			currPos += direction[i];
		}
	}
}


utils::Pattern GameGrid::findPattern(sf::Vector2i position)
{
	utils::Pattern hPattern;
	utils::PatternType hPatternResult = findHorizontalPattern(position, hPattern);
	utils::Pattern vPattern;
	utils::PatternType vPatternResult = findVerticalPattern(position, vPattern);
	utils::Pattern bPattern;
	utils::PatternType bPatternResult = findBoxPattern(position, bPattern);

	std::vector<std::pair<utils::Pattern, utils::PatternType>> patterns;
	patterns.push_back({ bPattern, bPatternResult });
	patterns.push_back({ hPattern, hPatternResult });
	patterns.push_back({ vPattern, vPatternResult });

	utils::Pattern result;
	bool hasBomb = false;
	for (auto& it : patterns)
	{
		if (it.second != utils::PatternType::NONE && it.second != utils::PatternType::MATCH)
		{
			for (int i = 0; i < it.first.size(); ++i)
			{
				result.push_back(it.first[i]);
			}

			if (hasBomb == false)
			{

				this->_gems[position.x][position.y].setPieceType(utils::toPieceType(it.second));
				hasBomb = true;
			}
		}
	}
	for (auto & it : patterns)
	{
		if (it.second == utils::PatternType::MATCH)
		{
for (int i = 0; i < it.first.size(); ++i)
{
	result.push_back(it.first[i]);
}
		}
	}
	if (hasBomb == true)
	{
		result.erase(std::remove(result.begin(), result.end(), position), result.end());
	}
	return result.size() > 0 ? result : utils::NULL_PATTERN;
}


utils::PatternType GameGrid::findHorizontalPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	pattern.push_back(position);
	collectPattern(position, pattern, utils::H_PATTERN_DIR);
	clearMarked();
	if (pattern.size() < 3) { return utils::PatternType::NONE; }
	return (pattern.size() > 3 ? utils::PatternType::H_BOMB : utils::PatternType::MATCH);
}

utils::PatternType GameGrid::findVerticalPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	pattern.push_back(position);
	collectPattern(position, pattern, utils::V_PATTERN_DIR);
	clearMarked();
	if (pattern.size() < 3) { return utils::PatternType::NONE; }
	return (pattern.size() > 3 ? utils::PatternType::V_BOMB : utils::PatternType::MATCH);
}

utils::PatternType GameGrid::findBoxPattern(sf::Vector2i position, utils::Pattern & pattern) const
{
	pattern.push_back(position);
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
	if (pattern.size() == 5 && std::abs(static_cast<int>(xCount.size()) - static_cast<int>(yCount.size())) == 1)
	{
		return utils::PatternType::R_BOMB;
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
			this->_gems[position.x][position.y].getPieceType() == this->_gems[newPosition.x][newPosition.y].getPieceType() &&
			this->_markedGems[newPosition.x][newPosition.y] == false)
		{
			pattern.push_back(newPosition);
			collectPattern(newPosition, pattern, direction);
		}
	}
}

void GameGrid::shuffle()
{
	for (int i = 0; i < this->_rows; ++i)
	{
		for (int j = 0; j < this->_columns; ++ j)
		{
			swap(this->_gems[i][j], this->_gems[rand() % this->_rows][rand() % this->_columns]);
		}
	}
}

bool GameGrid::findPossibleMatch(utils::Pattern & pattern)
{
	for (int i = 0; i < this->_rows; ++i)
	{
		for (int j = 0; j < this->_columns; ++j)
		{
			utils::Pattern pat;
			if (findHorizontalMatch(sf::Vector2i(i, j), pat))
			{
				printPattern(pat);
				pattern = pat;
				return true;
			}
			pat = utils::NULL_PATTERN;
			if (findVerticalMatch(sf::Vector2i(i, j), pat))
			{
				printPattern(pat);
				pattern = pat;
				return true;
			}
		}
	}
	return false;
}

bool GameGrid::findHorizontalMatch(sf::Vector2i position, utils::Pattern & pattern)
{
	pattern.push_back(position);
	this->_markedGems[position.x][position.y] = true;
	std::vector<sf::Vector2i> hDir = utils::H_PATTERN_DIR;
	std::vector<sf::Vector2i> vDir = utils::V_PATTERN_DIR;

	for (int i = 0; i < hDir.size(); ++i)
	{
		sf::Vector2i newPosition = position + hDir[i];
		while (0 <= newPosition.x && newPosition.x < this->_rows &&
			0 <= newPosition.y && newPosition.y < this->_columns &&
			this->_gems[position.x][position.y].getPieceType() == this->_gems[newPosition.x][newPosition.y].getPieceType() &&
			this->_markedGems[newPosition.x][newPosition.y] == false)
		{
			pattern.push_back(newPosition);
			this->_markedGems[newPosition.x][newPosition.y] = true;
			newPosition += hDir[i];
		}
		for (int j = 0; j < vDir.size(); ++j)
		{
			sf::Vector2i diagPos = newPosition + vDir[i];
			if (0 <= diagPos.x && diagPos.x < this->_rows &&
				0 <= diagPos.y && diagPos.y < this->_columns &&
				this->_gems[position.x][position.y].getPieceType() == this->_gems[diagPos.x][diagPos.y].getPieceType() &&
				this->_markedGems[diagPos.x][diagPos.y] == false)
			{
				pattern.push_back(diagPos);
				this->_markedGems[diagPos.x][diagPos.y] = true;
			}
		}
	}

	clearMarked();
	return pattern.size() >= 3;
}

bool GameGrid::findVerticalMatch(sf::Vector2i position, utils::Pattern & pattern)
{
	pattern.push_back(position);
	this->_markedGems[position.x][position.y] = true;
	std::vector<sf::Vector2i> vDir = utils::V_PATTERN_DIR;
	std::vector<sf::Vector2i> hDir = utils::H_PATTERN_DIR;

	for (int i = 0; i < vDir.size(); ++i)
	{
		sf::Vector2i newPosition = position + vDir[i];
		while (0 <= newPosition.x && newPosition.x < this->_rows &&
			0 <= newPosition.y && newPosition.y < this->_columns &&
			this->_gems[position.x][position.y].getPieceType() == this->_gems[newPosition.x][newPosition.y].getPieceType() &&
			this->_markedGems[newPosition.x][newPosition.y] == false)
		{
			pattern.push_back(newPosition);
			this->_markedGems[newPosition.x][newPosition.y] = true;
			newPosition += vDir[i];
		}
		for (int j = 0; j < hDir.size(); ++j)
		{
			sf::Vector2i diagPos = newPosition + hDir[i];
			if (0 <= diagPos.x && diagPos.x < this->_rows &&
				0 <= diagPos.y && diagPos.y < this->_columns &&
				this->_gems[position.x][position.y].getPieceType() == this->_gems[diagPos.x][diagPos.y].getPieceType() &&
				this->_markedGems[diagPos.x][diagPos.y] == false)
			{
				pattern.push_back(diagPos);
				this->_markedGems[diagPos.x][diagPos.y] = true;
			}
		}
	}

	clearMarked();
	return pattern.size() >= 3;
}

void GameGrid::checkColumn(int columnIndex)
{
	for(int i = 0; i < this->_gems.size(); ++i)
	{
		utils::Pattern tmpPattern = findPattern(sf::Vector2i(i, columnIndex));
		for(auto& it: tmpPattern)
		{
			this->_affectedGems[it.x][it.y] = true;
			this->_tiles[it.x][it.y].turnOnAffected();
			updateObjectives(this->_gems[it.x][it.y].getPieceType());
		}
	}
}

bool GameGrid::updateAffectedColumns()
{
	bool hasAffectedColumns = false;
	for(int i = 0; i < this->_affectedColumns.size(); ++i)
	{
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
		this->_affectedColumns[i] = true;
		if(this->_affectedColumns[i] == true)
		{
			checkColumn(i);
		}
	}
}

void GameGrid::updateObjectives(utils::PieceType PieceType)
{
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
		if (this->_affectedColumns[j] == true)
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
				this->_tiles[j - 1][columnIndex].turnOnAffected();
				this->_tiles[j][columnIndex].reset();
				
				j++;
			}
		}
	}
}

void GameGrid::dropAffectedColumns()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		if (this->_affectedColumns[i] == true)
		{
			dropDown(i);
		}
	}
}

void GameGrid::generateNewGems()
{
	for (int i = 0; i < this->_affectedColumns.size(); ++i)
	{
		if (this->_affectedColumns[i] == true)
		{
			int initPos = 0;
			while (initPos < this->_rows && this->_affectedGems[initPos][i] == true)
			{
				this->_gems[initPos][i].setImage(utils::randomGemType(this->_objectives, this->_figuresColorCount));
				this->_affectedGems[initPos][i] = false;
				this->_tiles[initPos][i].reset();
				++initPos;
			}
		}
	}
}
