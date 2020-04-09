#include "GameController.h"

GameController::GameController()
	: _gameStatus(GameStatus::Playing)
	, _app(nullptr)
	, _grid(nullptr)
	, _objectives(nullptr)
	, _turns(nullptr)
	, _gameOverWindow(nullptr)
	, _mouse_pos(-1, -1)
	, _clickCount(0)
	, _isClicked(false)
	, _clickedGems(utils::NULL_GEM_PAIR)
{
}

void GameController::parseConfigFile(const std::string & configFilename)
{
	Json::Reader reader;
	Json::Value root;
	if (isCorrectConfigFile(configFilename, reader, root))
	{
		std::vector<std::pair<utils::PieceType, int>> objectiveGems;
		for (std::size_t i = 0; i < root["objectives"].size(); ++i)
		{
			std::string PieceType = root["objectives"][(int)i][0].asString();
			int count = root["objectives"][(int)i][1].asInt();
			objectiveGems.push_back({ utils::getPieceType(PieceType), count });
		}
		this->_objectives = std::make_unique<Objectives>(objectiveGems);
	}
}

GameController::~GameController()
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
			if (!root["board_row_size"].isInt() || utils::MIN_ROWS > root["board_row_size"].asInt()
				|| root["board_row_size"].asInt() > utils::MAX_ROWS)
			{
				throw std::logic_error("Not correct board_row_size in config file.\n");
			}
			if (!root["board_column_size"].isInt() || utils::MIN_COLUMNS > root["board_column_size"].asInt()
				|| root["board_column_size"].asInt() > utils::MAX_COLUMNS)
			{
				throw std::logic_error("Not correct board_column_size in config file.\n");
			}
			if (!root["objectives"].isArray() || utils::MIN_OBJECTIVES_COUNT > root["objectives"].size()
				|| root["objectives"].size() > utils::MAX_OBJECTIVES_COUNT)
			{
				throw std::logic_error("Not correct objectives in config file.\n");
			}
			if (!root["figures_colors_count"].isInt() || utils::MIN_FIGURES_COUNT > root["figures_colors_count"].asInt()
				|| root["figures_colors_count"].asInt() > utils::MAX_FIGURES_COUNT)
			{
				throw std::logic_error("Not correct figures_colors_count in config file.\n");
			}
			int figures_count = root["figures_colors_count"].asInt();
			int obj_size = root["objectives"].size();
			if (root["figures_colors_count"].asInt() < static_cast<int>(root["objectives"].size()))
			{
				throw std::logic_error("Not enough figures_colors_count to set objectives.\n");
			}
			std::vector<utils::PieceType> objectiveGems;
			for (std::size_t i = 0; i < root["objectives"].size(); ++i)
			{
				if (root["objectives"][(int)i][0].isString() == false
					|| root["objectives"][(int)i][1].isInt() == false
					|| root["objectives"][(int)i][1].asInt() < 0)
				{
					throw std::logic_error("Specified objectives are not correct.\n");
				}
				std::string type = root["objectives"][(int)i][0].asString();
				objectiveGems.push_back(utils::getPieceType(type));
			}
			for (int i = 0; i < objectiveGems.size(); ++i)
			{
				if (objectiveGems[i] == utils::PieceType::UNKNOWN)
				{
					throw std::logic_error("Found unknown gem type in objectives.\n");
				}
			}
			for (int i = 0; i < objectiveGems.size(); ++i)
			{
				for (int j = i + 1; j < objectiveGems.size(); ++j)
				{
					if (objectiveGems[i] == objectiveGems[j])
					{
						throw std::logic_error("Found duplicate gem type in objectives.\n");
					}
				}
			}
		}
		else
		{
			throw std::logic_error(reader.getFormattedErrorMessages());
		}
	}
	return true;
}

void GameController::startGame() 
{
	initialize(utils::CONFIG_FILENAME);
    run();
}

void GameController::update()
{
	if (this->_grid->getStatus() == GridStatus::WAITING)
	{
		this->_objectives->updateObjectives(this->_grid->getUpdatedObjectives());
	}
	this->_grid->updateStatus();	
}

void GameController::draw()
{
	if (this->_gameStatus != GameStatus::Playing)
	{
		this->_app->draw(*this->_gameOverWindow);
		return;
	}
	this->_app->draw(*this->_objectives);
	this->_app->draw(*this->_turns);
	this->_app->draw(*this->_grid);
}

void GameController::run() 
{
    while (this->_app->isOpen()) 
    {
		updateStatus();
		this->_app->clear(utils::GAME_CONTROLLER_COLOR);

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
					this->_isClicked = true;
					++this->_clickCount;
					this->_mouse_pos = sf::Mouse::getPosition(*_app);
					updateClickedGems();
				}
			}
        }
		
        update();
		draw();
		this->_app->display();
     }
}

void GameController::initialize(const std::string & configFilename)
{
	parseConfigFile(configFilename);
	this->_grid = std::make_unique<GameGrid>(configFilename, this->_objectives->getObjectives());
	this->_turns = std::make_unique<Turns>(configFilename);
	this->_gameOverWindow = std::make_unique<GameOverWindow>();
	this->_app = std::make_unique<sf::RenderWindow>(sf::VideoMode(utils::WINDOW_WIDTH, utils::WINDOW_HEIGHT), "Match3Game", sf::Style::Close);
	this->_app->setFramerateLimit(utils::MAX_FRAMERATE_LIMIT);
}

void GameController::updateClickedGems()
{
	if (this->_clickCount == 0)
	{
		return;
	}
	sf::Vector2i currentGem = this->_grid->getGridIndices(this->_mouse_pos);

	if (currentGem.x < 0 || currentGem.x >= this->_grid->getRows() 
		|| currentGem.y < 0 || currentGem.y >= this->_grid->getColumns())
	{
		--this->_clickCount;
		return;
	}
	if (this->_clickCount == 1)
	{
		this->_clickedGems.first = currentGem;
		this->_grid->toggleTile(this->_clickedGems.first);
		if (this->_grid->isBomb(currentGem) == true)
		{
			std::cout << "clicked bomb: "  << " " << currentGem.x << " " << currentGem.y << "\n";
			this->_grid->detonateBomb(currentGem);
			this->_grid->setStatus(GridStatus::CHECKING);
			this->_turns->updateTurns();
			this->_grid->toggleTile(this->_clickedGems.first);
			--this->_clickCount;
		}
	}
	else if (this->_clickCount == 2)
	{
		this->_clickedGems.second = currentGem;
		std::cout << "clicked gems\n";
		std::cout << "gem 1: " << this->_clickedGems.first.x << " " << this->_clickedGems.first.y << "\n";
		std::cout << "gem 2: " << " " << this->_clickedGems.second.x << " " << this->_clickedGems.second.y << "\n";
		//this->_grid->toggleTile(this->_clickedGems.first);
		this->_grid->toggleTile(this->_clickedGems.second);
		
		if (std::abs(this->_clickedGems.first.x - this->_clickedGems.second.x) + std::abs(this->_clickedGems.first.y - this->_clickedGems.second.y) == 1
			&& this->_grid->isBomb(currentGem) != true)
		{
			bool isSuccessfuTurn = this->_grid->swapGems(this->_clickedGems);
			if (isSuccessfuTurn)
			{
				this->_turns->updateTurns();
			}
		}
		this->_grid->toggleTile(this->_clickedGems.first);
		this->_grid->toggleTile(this->_clickedGems.second);
		this->_clickedGems = utils::NULL_GEM_PAIR;
		this->_clickCount -= 2;
	}
}


void GameController::updateStatus()
{
	switch (this->_gameStatus)
	{
	case GameStatus::Playing:
	{
		bool areTurnsLeft = this->_turns->areTurnsLeft();
		bool areObjectivesLeft = this->_objectives->areObjectivesLeft();
		if (areTurnsLeft == false && areObjectivesLeft == true)
		{
			this->_gameStatus = GameStatus::Failed;
		}
		else if (areObjectivesLeft == false)
		{
			this->_gameStatus = GameStatus::Won;
		}
		return;
	}
	case GameStatus::Won:
	{
		this->_gameOverWindow->haveWon(true);
		return;
	}
	case GameStatus::Failed:
	{
		this->_gameOverWindow->haveWon(false);
		return;
	}
	}
}