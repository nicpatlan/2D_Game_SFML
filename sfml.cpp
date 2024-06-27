#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <iterator>
#include "include/Aim.h"
#include "include/Water.h"
#include "include/Ship.h"
#include "include/Cannon.h"

void gameLoop(sf::RenderWindow &window, 
              int boat_time, int max_bullets,
              int max_x, int max_y) {
    // game components
    Aim cursor = Aim();
    Water water = Water();
    std::vector<Ship> ships;
    Cannon cannon = Cannon(max_x, max_y, max_bullets);

    // timing component
    sf::Clock clk;
    float total_time = 0;

    // TODO seed with a real random value
    std::random_device r;
 
    // Choose a random integer between 1 and 6
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 6);

    // subdivisions of y by row
    int row_height = max_y / 7;

    while (window.isOpen()) {
        float elapsed_time = clk.restart().asSeconds();
        total_time += elapsed_time;
        if (total_time > boat_time) {
            int rand_num = uniform_dist(e1);
            int ship_posy = rand_num * row_height;
            Ship *ship = new Ship(ship_posy);
            ships.push_back(*ship);
            total_time = 0;
        }    

        sf::Event event;
        int mouse_posx = sf::Mouse::getPosition(window).x;
        int mouse_posy = sf::Mouse::getPosition(window).y;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                sf::Keyboard::Scan::Scancode keyCode = event.key.scancode;
                if (keyCode == sf::Keyboard::Scan::Escape) {
                    // exit
                    window.close();
                } else if (keyCode == sf::Keyboard::Scan::Space) {
                    // shoot
                    cannon.shootBullet(mouse_posx, mouse_posy);
                }
            }
        }

        // set position of the circle based on mouse position
        cursor.setAim(mouse_posx, mouse_posy);

        //draw elements
        window.clear();
        window.draw(water.getBackground());
        window.draw(cursor.getAim());
        window.draw(cannon.getCannon());

        // handle ships
        std::vector<Ship>::iterator it;        
        for (it = ships.begin(); it < ships.end(); it++) {
            window.draw(it->getSprite());
            // update ship position
            it->moveSprite(elapsed_time * 100.0);
            if (it->getSprite().getPosition().x > max_x) {
                ships.erase(it);
            } else if (cannon.checkCollision(*it, max_x, max_y)) {
                ships.erase(it);
            }
         }

        //handle bullets
        cannon.moveBullets(window, elapsed_time);
        cannon.checkBulletBoundaries(max_x, max_y);

        window.display();
    }
}

int main() {
    // find desktop dimensions & build main game window
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int max_x = desktop.width / 2;
    unsigned int max_y = desktop.height / 2;
    auto window = sf::RenderWindow(sf::VideoMode({max_x, max_y}), "Ship Shooter", sf::Style::None);
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // start the game loop
    int boat_spawn_time = 3;
    int max_bullet_count = 5;
    gameLoop(window, boat_spawn_time, max_bullet_count, max_x, max_y);

    return 0;
}
