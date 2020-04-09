#ifndef __GAMECONTROLLER_HPP__
#define __GAMECONTROLLER_HPP__

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <exception>
#include <SFML/Graphics.hpp>
#include "GameGrid.h"
#include "Objectives.h"
#include "Turns.h"
#include "GameOverWindow.h"
#include "utils.h"

enum GameStatus {
    Playing,
    Won,
    Failed
};

class GameController {

public :
    GameController();

	void startGame();
private:
    GameStatus _gameStatus;
	std::unique_ptr<sf::RenderWindow> _app;
	std::unique_ptr<GameGrid> _grid;
	std::unique_ptr<Objectives> _objectives;
	std::unique_ptr<Turns> _turns;
	std::unique_ptr<GameOverWindow> _gameOverWindow;

	sf::Vector2i _mouse_pos;
	int _clickCount;
	bool _isClicked;
	utils::GemPair _clickedGems;

	~GameController();
private:
	bool isCorrectConfigFile(const std::string& configFilename, Json::Reader& reader, Json::Value& root);
    void parseConfigFile(const std::string& configFilename);
    void run();
	void update();
	void draw();
	void initialize(const std::string& configFilename);
	void updateClickedGems();
	void updateStatus();

};

#endif // __GAMECONTROLLER_HPP__