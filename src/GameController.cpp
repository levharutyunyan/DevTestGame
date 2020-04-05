#include "GameController.hpp"
//#include "utils.hpp"

GameController::GameController()
	: _gameStatus(NotStarted)
	, _grid(nullptr)
	, _mouse_pos(0, 0)
	, _shouldMove(false)
	, _shouldSwap(false)
	, _clickCount(0)
{
	
}

void GameController::updateGameStatus(GameStatus &status) 
{
    _gameStatus = status;
}

bool GameController::shouldMove() const
{
	return this->_shouldMove;
}

bool GameController::shouldSwap() const
{
	return this->_shouldSwap;
}

void GameController::startGame() {
    _app = new sf::RenderWindow(sf::VideoMode(utils::WINDOW_WIDTH, utils::WINDOW_HEIGHT), "Match3Game", sf::Style::Close);
    _app->setFramerateLimit(utils::MAX_FRAMERATE_LIMIT);
    run();
}

void GameController::run() {

	std::vector<std::pair<int, utils::GemType>> objectives{ {10, utils::GemType::BLUE},
													{12, utils::GemType::GREEN},
													{20, utils::GemType::RED} };
	GameGrid grid(utils::CONFIG_FILENAME, utils::GRID_BACKGROUND_FILENAME, &objectives);
	this->_grid = &grid;

    while (this->_app->isOpen()) {
        this->_app->clear(sf::Color(50, 150, 150, 255));
		sf::Event event;
        while (this->_app->pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed)
			{
                this->_app->close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.key.code == sf::Mouse::Left)
				{
					++this->_clickCount;
					std::cout << "mouse clicked: " << this->_clickCount << "\n";
					this->_mouse_pos = sf::Mouse::getPosition(*_app);
				}
			}
        }
		updateClickedGems(this->_mouse_pos);

		this->_app->draw(grid);
        this->_app->display();
    }
}

void GameController::updateClickedGems(const sf::Vector2i& mouse_pos)
{
	sf::Vector2i currentGem = this->_grid->getGridIndices(mouse_pos);
	std::cout << "click: " << this->_clickCount << "\n";
	if (this->_clickCount == 1)
	{
		this->_clickedGems.first = currentGem;
		this->_clickedGems.second = utils::NULL_POS;
	}
	else if (this->_clickCount == 2)
	{
		this->_clickedGems.second = currentGem;
		this->_clickCount -= 2;
		this->_grid->swapGems(this->_clickedGems.first, this->_clickedGems.second);

	}
	/*std::cout << "----\n";
	std::cout << "selected gems\n";
	std::cout << "first " << this->_clickedGems.first.x << " " << this->_clickedGems.first.y << "\n";
	std::cout << "second " << this->_clickedGems.second.x << " " << this->_clickedGems.second.y << "\n";

	std::cout << "----\n";*/
}

void GameController::updateStatus()
{
	//NotStarted
	//Started
	//Won
	//Lost
}
//
//void GameController::checkForAction()
//{
//	if (this->_clickCount == 0)
//	{
//		return;
//	}
//	if (this->_clickCount == 1)
//	{
//		this->_grid->getGridIndices(this->_mouse_pos);
//		std::cout << "Clicked first time\n";
//	}
//	else if (this->_clickCount == 2)
//	{
//
//	}
//}