#ifndef __GAMEGRID_HPP__
#define __GAMEGRID_HPP__

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "json/json.h"
#include "utils.hpp"

enum class GridStatus : int
{
	NONE, CREATING, CHECKING, DELETING, MOVING
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
	struct Gem : public sf::Drawable, public sf::Transformable
	{
		int _width;
		int _height;
		std::string _filename;
		sf::Image _image;
		sf::Texture _texture;
		sf::Sprite _sprite;
		bool _isDeleted;
		Gem(int width, int height);

		void setImage(const std::string& image_filename);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void pop();
		static std::string randomGemFilename(const std::vector<std::pair<int, utils::GemType>>* objs, int figures_colors_count);
	};
private:
	using Tiles = std::vector<std::vector<Tile>>;
	using Gems = std::vector<std::vector<Gem>>;
	using Objectives = std::vector<std::pair<int, utils::GemType>>;
	using AffectedGems = std::vector<std::vector<bool>>;
public:
	GameGrid(const std::string& json_filename, const sf::String& bkgr_filename,
		std::vector<std::pair<int, utils::GemType>>* objectives);
	GameGrid();
private:

	GridStatus _gridStatus;
	int _rows;
	int _columns;
	Tiles _tiles;
	Gems _gems;
	AffectedGems _affectedGems;
	int _figuresColorCount;
	std::vector<int> _affectedColumns;
	
	float _gridXPos;
	float _gridYPos;
	float _gridXScale;
	float _gridYScale;

	std::vector<std::pair<int, utils::GemType>>* _objectives;
	sf::Image _bkgrImage;
	sf::Texture _bkgrTexture;
	sf::Sprite _bkgrSprite;
	//RectangleShape _gridShape; 

public:
	//float gridXPos() const;
	//float gridYpos() const;
	void updateGrid();
	
	void swapGems(const sf::Vector2i& lhs_pos, const sf::Vector2i& rhs_pos);
	sf::Vector2i getGridIndices(const sf::Vector2i& mouse_pos) const;
	utils::Pattern checkForMatch(const sf::Vector2i& position);
	
private:
	bool isCorrectJsonFile(const std::string& json_filename,
		Json::Reader& reader, Json::Value& root) const;
	bool parseJsonFile(const std::string& json_filename);
	bool getGraphicsElements(const std::string& bkgr_filename);
	void fillTiles();
	void fillGems();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void findPattern(sf::Vector2i position, utils::Pattern& pattern, const sf::Vector2i& direction);
	static bool matchPattern(const utils::Pattern& pattern);
	void popGems(const utils::Pattern& pattern);
	void checkColumn(int columnIndex);
	void dropDown(int columnIndex);
	void dropColumns();
};

#endif //__GAMEGRID_HPP__