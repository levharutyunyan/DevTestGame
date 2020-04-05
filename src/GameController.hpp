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

    void startGame();

    void updateGameStatus(GameStatus &status);

	bool shouldMove() const;
	bool shouldSwap() const;
private:
    GameStatus _gameStatus = NotStarted;
    sf::RenderWindow *_app;
	GameGrid *_grid;
	sf::Vector2i _mouse_pos;
	bool _shouldMove;
	bool _shouldSwap;
	int _clickCount;
	std::pair<sf::Vector2i, sf::Vector2i> _clickedGems;

private:
    void run();
	void updateClickedGems(const sf::Vector2i& mouse_pos);
	void updateStatus();

};

#endif // __GAMECONTROLLER_HPP__