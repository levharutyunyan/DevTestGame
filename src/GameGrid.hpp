#ifndef __GAMEGRID_HPP__
#define __GAMEGRID_HPP__

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "Tile.hpp"
#include "json/json.h"
#include "utils.hpp"

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
	using AffectedGems = std::vector<std::vector<bool>>;
public:
	GameGrid(const std::string& json_filename,
		const std::vector<std::pair<utils::PieceType, int>>& objectives);
	GameGrid();
private:
	float _gridXPos;
	float _gridYPos;
	int _rows;
	int _columns;
	int _figuresColorCount;
	GridStatus _gridStatus;
	bool _isSuccessfulTurn;

	Tiles _tiles;
	Gems _gems;
	AffectedGems _affectedGems;
	std::vector<bool> _affectedColumns;
	std::vector<std::pair<utils::PieceType, int>> _objectives;
	/*utils::GemPair _gemPair;*/
	// float _gridXScale;
	// float _gridYScale;

	// sf::Image _bkgrImage;
	// sf::Texture _bkgrTexture;
	// sf::Sprite _bkgrSprite;
	//RectangleShape _gridShape; 

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
	void printPattern(const utils::Pattern& pattern);
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

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void searchByDirection(sf::Vector2i position, utils::Pattern& pattern, const sf::Vector2i& direction);
	void markGems(const utils::Pattern& pattern);
	
	// Grid status == CHECKING
	utils::Pattern checkForMatch(const sf::Vector2i& position);
	void checkColumn(int columnIndex);
	bool updateAffectedColumns();
	void checkAffectedColumns();
	
	// Grid status == DELETING
	void updateObjectives(utils::PieceType type);
	void deleteAffectedGems();

	// Grid status == MOVING
	void dropAffectedColumns();
	void dropDown(int columnIndex);
	
	//void rearangeGemsPositions();
	//void dropAffectedColumns();

	// Grid status == CREATING
	void generateNewGems();

};

#endif //__GAMEGRID_HPP__