#ifndef __GAMEGRID_HPP__
#define __GAMEGRID_HPP__

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "Gem.hpp"
#include "json/json.h"
#include "utils.hpp"

enum class GridStatus : int
{
	NONE, CREATING, CHECKING, DELETING, MOVING, SWAPPING, WAITING
};

class GameGrid : public sf::Drawable, public sf::Transformable
{
private: 
	struct Tile : public sf::Drawable, public sf::Transformable
	{
		int _width;
		int _height;
		sf::Image _image;
		sf::Texture _texture;
		sf::Sprite _sprite;

		Tile(int width, int height);
		
		void setImage(const std::string& image);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
	// TODO: Implement Gem factory and Tile factory
	/*struct Gem : public sf::Drawable, public sf::Transformable
	{
		int _width;
		int _height;
		std::string _filename;
		sf::Image _image;
		sf::Texture _texture;
		sf::Sprite _sprite;
		
		Gem(int width, int height);

		void setImage(const std::string& image_filename);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		friend void swap(Gem& lhs, Gem& rhs);
		void pop();
		utils::GemType getGemType();
		static std::string randomGemFilename(const std::vector<std::pair<int, utils::GemType>>* objs, int figures_colors_count);
		char getShortFilename() const;
	};*/
private:
	using Tiles = std::vector<std::vector<Tile>>;
	using Gems = std::vector<std::vector<Gem>>;
	using Objectives = std::vector<std::pair<int, utils::GemType>>;
	using AffectedGems = std::vector<std::vector<bool>>;
public:
	GameGrid(const std::string& json_filename,
		std::vector<std::pair<int, utils::GemType>>* objectives);
	GameGrid();
private:
	float _gridXPos;
	float _gridYPos;
	int _rows;
	int _columns;
	int _figuresColorCount;
	GridStatus _gridStatus;

	Tiles _tiles;
	Gems _gems;
	AffectedGems _affectedGems;
	std::vector<bool> _affectedColumns;
	std::vector<std::pair<int, utils::GemType>>* _objectives;
	utils::GemPair _gemPair;
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
	void swapGems();
	
	//Grid status == WAITING
	void setGemPair(const utils::GemPair& gemPair);
	
	void setStatus(GridStatus status);
	
private:
	// bool isCorrectJsonFile(const std::string& json_filename,
		// Json::Reader& reader, Json::Value& root) const;
	bool parseJsonFile(const std::string& json_filename);
	void getGraphicsElements();
	void fillTiles();
	void fillGems();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void findPattern(sf::Vector2i position, utils::Pattern& pattern, const sf::Vector2i& direction);
	void popGems(const utils::Pattern& pattern);
	
	// Grid status == CHECKING
	utils::Pattern checkForMatch(const sf::Vector2i& position);
	void checkColumn(int columnIndex);
	bool updateAffectedColumns();
	void checkAffectedColumns();
	
	// Grid status == DELETING
	void updateObjective(utils::GemType);
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