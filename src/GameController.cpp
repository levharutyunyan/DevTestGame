#include "GameController.hpp"
//#include "utils.hpp"

GameController::GameController()
	: _gameStatus(NotStarted)
	, _grid(nullptr)
	, _mouse_pos(-1, -1)
	, _clickCount(0)
	, _clickedGems(utils::NULL_GEM_PAIR)
{
}

void GameController::updateGameStatus(GameStatus &status) 
{
    _gameStatus = status;
}

void GameController::parseConfigFile(const std::string & configFilename)
{
}

bool GameController::isCorrectConfigFile(const std::string & configFilename, Json::Reader & reader, Json::Value & root)
{
	std::ifstream config_doc(utils::CONFIG_FILENAME);
	if (config_doc.is_open())
	{
		bool parseSuccess = reader.parse(config_doc, root, false);
		if (parseSuccess)
		{
			if (!root["board_row_size"].isInt() || 7 > root["board_row_size"].asInt()
				|| root["board_row_size"].asInt() > 10)
			{
				return false;
			}
			if (!root["board_column_size"].isInt() || 7 > root["board_column_size"].asInt()
				|| root["board_column_size"].asInt() > 10)
			{
				return false;
			}
			if (!root["objectives"].isArray() || 1 > root["objectives"].size()
				|| root["objectives"].size() > 3)
			{
				return false;
			}
			if (!root["figures_colors_count"].isInt() || 3 > root["figures_colors_count"].asInt()
				|| root["figures_colors_count"].asInt() > 5)
			{
				return false;
			}

			//TODO if(undefined_color)
		}
		else
		{
			std::cout << reader.getFormatedErrorMessages();
			std::cout << reader.getFormattedErrorMessages();
			return false;
		}
	}
	
	return true;
}

void GameController::startGame() {
    _app = new sf::RenderWindow(sf::VideoMode(utils::WINDOW_WIDTH, utils::WINDOW_HEIGHT), "Match3Game", sf::Style::Close);
    _app->setFramerateLimit(utils::MAX_FRAMERATE_LIMIT);
    run();
}

void GameController::update()
{
	this->_grid->updateStatus();	
	updateClickedGems();
}

void GameController::run() {

	std::vector<std::pair<int, utils::GemType>> objectives{ {10, utils::GemType::BLUE},
													{12, utils::GemType::GREEN},
													{20, utils::GemType::RED} };
	GameGrid grid(utils::CONFIG_FILENAME, &objectives);
	this->_grid = &grid;
    while (this->_app->isOpen()) 
    {
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
					// std::cout << "mouse clicked: " << this->_clickCount << "\n";
					this->_mouse_pos = sf::Mouse::getPosition(*_app);
				}
			}
        }
        update();
		
		this->_app->draw(grid);
		this->_app->display();
     }
}

void GameController::updateClickedGems()
{
	if (this->_clickCount == 0)
	{
		return;
	}
	sf::Vector2i currentGem = this->_grid->getGridIndices(this->_mouse_pos);
	std::cout << "updating clickedGems\n";
	std::cout << "click: " << this->_clickCount << " " << currentGem.x << " " << currentGem.y << "\n";
	if (this->_clickCount == 1)
	{
		this->_clickedGems.first = currentGem;
	}
	else if (this->_clickCount == 2)
	{
		this->_clickedGems.second = currentGem;
		if (std::abs(this->_clickedGems.first.x - this->_clickedGems.second.x) + std::abs(this->_clickedGems.first.y - this->_clickedGems.second.y) != 1)
		{
			this->_clickedGems = utils::NULL_GEM_PAIR;
		}
		else
		{
			this->_grid->setGemPair(this->_clickedGems);
		}
		this->_grid->swapGems();
		this->_clickCount -= 2;
	}
}

void GameController::updateStatus()
{
	//NotStarted
	//Started
	//Won
	//Lost
}