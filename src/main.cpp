#include <iostream>
#include "GameController.hpp"

int main() {
    GameController* gc = new GameController();
    gc->runLoop();
    return 0;
}
