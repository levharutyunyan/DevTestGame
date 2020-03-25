//
//  GameController.cpp
//  SFMLTest
//
//  Created by David Aleksanyan on 3/23/20.
//  Copyright © 2020 David Aleksanyan. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include "GameController.hpp"

using namespace sf;

GameController::GameController() {
}

void GameController::runLoop() {
    RenderWindow app(VideoMode(744, 1080), "My Game", Style::Close);
    app.setFramerateLimit(60);

    Texture texture;
    texture.loadFromFile("/Users/davidaleksanyan/Projects/cpp/SFMLTest/Resources/bomb.png");
    Sprite sprite(texture);
    sprite.setPosition(100,100);

    while (app.isOpen()) {
        app.clear(Color(255,255,255,255));
        app.draw(sprite);
        sf::Event event;
        while (app.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                app.close();
        }
        app.display();
    }
}