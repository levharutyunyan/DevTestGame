#include <time.h>
#include <iostream>
#include <vector>
#include <functional>
#include <SFML/System.hpp>
#include <exception>
#include "GameController.h"
#include "utils.h"
#include "Textures.h"

int sum(int a, char b)
{
	return 2;
}

int main() 
{
	srand(NULL);
	try {
		std::cout << "Loading Textures...\n";
		utils::Textures::populateTextures();
		std::cout << "Textures are loaded.\n";
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
	
	std::vector<std::function<int(int, char)> > functions;
	functions.push_back(sum);

	GameController* gc = new GameController();
	gc->startGame();
	return 0;
}
