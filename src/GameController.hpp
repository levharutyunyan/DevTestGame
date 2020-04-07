#ifndef __GAMECONTROLLER_HPP__
#define __GAMECONTROLLER_HPP__

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <SFML/Graphics.hpp>
#include "GameGrid.hpp"
#include "Objectives.hpp"
#include "Turns.hpp"
#include "GameOverWindow.hpp"
#include "utils.hpp"

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
	sf::RenderWindow *_app;
	GameGrid *_grid;
	Objectives *_objectives;
	Turns *_turns;
	GameOverWindow *_gameOverWindow;

	sf::Vector2i _mouse_pos;
	int _clickCount;
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