#include "GameController.hpp"
#include <time.h>
#include <SFML/System.hpp>

int main() 
{
	srand(NULL);
	//TODO load config
	GameController* gc = new GameController();
	gc->startGame();
	return 0;
}
