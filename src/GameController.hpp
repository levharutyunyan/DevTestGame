#ifndef __GAMECONTROLLER_HPP__
#define __GAMECONTROLLER_HPP__

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameGrid.hpp"
#include "utils.hpp"

enum GameStatus {
    NotStarted,
    Started,
    Won,
    Failed
};

class GameController {

public :
    GameController();

    void parseConfigFile(const std::string& configFilename);
	bool isCorrectConfigFile(const std::string& configFilename, Json::Reader& reader, Json::Value& root);
	void startGame();

    void updateGameStatus(GameStatus &status);

private:
    GameStatus _gameStatus = NotStarted;
    sf::RenderWindow *_app;
	GameGrid *_grid;
	sf::Vector2i _mouse_pos;
	int _clickCount;
	utils::GemPair _clickedGems;
	// TODO: Add here OBJEctives and make it with unordered_set
private:
    void run();
	void update();

	void updateClickedGems();
	void updateStatus();

};

#endif // __GAMECONTROLLER_HPP__