#include <time.h>
#include <iostream>
#include <vector>
#include <functional>
#include <SFML/System.hpp>
#include <exception>
#include "GameController.h"
#include "utils.h"
#include "Textures.h"


int main() 
{
	srand(NULL);
	try {
		// Doesn't do anything;
		std::cout << "Loading Textures...\n";
		utils::Textures::populateTextures();
		std::cout << "Textures are loaded.\n";
	
		GameController* gc = new GameController();
		gc->startGame();
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
	return 0;
}
