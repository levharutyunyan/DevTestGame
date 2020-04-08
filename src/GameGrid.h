#ifndef __GAMEGRID_HPP__
#define __GAMEGRID_HPP__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <set>

#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Tile.h"
#include "json/json.h"
#include "utils.h"

enum class GridStatus : int
{
	NONE, CREATING, CHECKING, DELETING, MOVING, SWAPPING, WAITING
};

class GameGrid : public sf::Drawable, public sf::Transformable
{
private:
	using Tiles = std::vector<std::vector<Tile>>;
	using Gems = std::vector<std::vector<Piece>>;
	using Objectives = std::vector<std::pair<int, utils::PieceType>>;
	using VectorBool = std::vector<std::vector<bool>>;
	using PatternFinder = std::function<utils::PatternType(sf::Vector2i position, utils::Pattern& pattern)>;
public:
	GameGrid(const std::string& json_filename,
		const std::vector<std::pair<utils::PieceType, int>>& objectives);

public:
	void updateStatus();
	sf::Vector2i getGridIndices(const sf::Vector2i& mouse_pos) const;

	//Grid status == SWAPPING;
	bool swapGems(const utils::GemPair& gemPair);
	
	//Grid status == WAITING
	//void setGemPair(const utils::GemPair& gemPair);
	std::vector<std::pair<utils::PieceType, int>> getUpdatedObjectives() const;
	
	GridStatus getStatus() const;
	void setStatus(GridStatus status);
	void printPattern(const utils::Pattern& pattern) const;
	int getRows() const;
	int getColumns() const;
	bool getTurnResult() const;
private:
	// bool isCorrectJsonFile(const std::string& json_filename,
		// Json::Reader& reader, Json::Value& root) const;
	bool parseJsonFile(const std::string& json_filename);
	void getGraphicsElements();
	void fillTiles();
	void fillGems();
	void clearMarked() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setAffected(const utils::Pattern& pattern);

private:
	//Bombs
	void detonateBomb(sf::Vector2i position);
	void detonate(sf::Vector2i position, const std::vector<sf::Vector2i>& direction);

private:
	//Patterns
	utils::Pattern findPattern(sf::Vector2i position);
	utils::PatternType findHorizontalPattern(sf::Vector2i position, utils::Pattern& pattern) const;
	utils::PatternType findVerticalPattern(sf::Vector2i position, utils::Pattern& pattern) const;
	utils::PatternType findBoxlPattern(sf::Vector2i position, utils::Pattern& pattern) const;
	void collectPattern(sf::Vector2i position, utils::Pattern& pattern, const std::vector<sf::Vector2i>& direction) const;
	
	void searchByDirection(sf::Vector2i position, utils::Pattern& pattern, const sf::Vector2i& direction);
private:
	// Grid status == CHECKING
	utils::Pattern checkForMatch(const sf::Vector2i& position);
	void checkColumn(int columnIndex);
	bool updateAffectedColumns();
	void checkAffectedColumns();
	
	// Grid status == DELETING
	void updateObjectives(utils::PieceType type);
	void noteAffectedGems();

	// Grid status == MOVING
	void dropAffectedColumns();
	void dropDown(int columnIndex);
	
	//void rearangeGemsPositions();
	//void dropAffectedColumns();

	// Grid status == CREATING
	void generateNewGems();

private:
	float _gridXPos;
	float _gridYPos;
	int _rows;
	int _columns;
	int _figuresColorCount;
	GridStatus _gridStatus;
	bool _isSuccessfulTurn;
	std::vector<PatternFinder> _patternFinders;

	Tiles _tiles;
	Gems _gems;
	VectorBool _affectedGems;
	mutable VectorBool _markedGems;
	std::vector<bool> _affectedColumns;
	std::vector<std::pair<utils::PieceType, int>> _objectives;

};

#endif //__GAMEGRID_HPP__