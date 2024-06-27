#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <iterator>

class Water {
    public:
        sf::Texture back_texture;
        sf::Sprite background;
        Water() {
            if (!back_texture.loadFromFile("src/assets/blueWater.png")) {
                std::cout << "error loading background" << std::endl;
            }
            background.setTexture(back_texture);
            background.scale(sf::Vector2f(5, 3));
        }
};

class Aim {
    public:
        sf::CircleShape aim;
        Aim() {
            aim = sf::CircleShape(5.f);
            aim.setFillColor(sf::Color::White);
            aim.setOrigin(10, 10);
        }
};

class Ship {
    public:
        sf::Texture ship_texture;
        sf::Sprite ship_sprite;
        Ship(int ship_posy, int ship_posx = 0) {
            if (!ship_texture.loadFromFile("src/assets/redShip.png")) {
                std::cout << "error loading sprite" << std::endl;
            }
            ship_sprite.setTexture(ship_texture);
            ship_sprite.setPosition(sf::Vector2f(0, ship_posy));
            ship_sprite.scale(sf::Vector2f(0.03, 0.03));
        }
};

class Cannon {
    public:
        sf::Texture cannon_texture;
        sf::Sprite cannon;
        int max_bullets;
        int bullet_idx;
        float bullet_velocity;
        std::vector<sf::CircleShape> bullets;
        std::vector<float> bullet_angles;
        std::vector<int> active_bullets;
        Cannon(int max_x, int max_y, int bullet_count) {
            // set up cannon
            if (!cannon_texture.loadFromFile("src/assets/cannon.png")) {
                std::cout << "error loading cannon" << std::endl;
            }
            cannon.setTexture(cannon_texture);
            int x = cannon.getLocalBounds().getSize().x;
            int y = cannon.getLocalBounds().getSize().y;
            cannon.setOrigin(x / 2, y / 2);
            cannon.setPosition(max_x / 2, max_y - 15);

            // set up bullets
            max_bullets = bullet_count;
            bullet_idx = 0;
            bullet_velocity = 100.0;
            for (int i = 0; i < max_bullets; i++) {
                sf::CircleShape bullet(5.f);
                bullet.setFillColor(sf::Color::Black);
                bullet.setOrigin(5, 5);
                bullet.setPosition(max_x / 2, max_y - 30);
                bullets.push_back(bullet);
                bullet_angles.push_back(0.f);
                active_bullets.push_back(0);
            }
        }

        void shootBullet(int mouse_posx, int mouse_posy) {
            for (int i = 0; i < max_bullets; i++) {
                if (!active_bullets[i]) {
                    bullet_idx = i;
                    break;
                } else if (i == max_bullets - 1) {
                    bullet_idx = -1;
                }
            }
            if (bullet_idx != -1) {
                active_bullets[bullet_idx] = 1;
                bullet_angles[bullet_idx] = std::atan2(mouse_posy - cannon.getPosition().y,
                                                       mouse_posx - cannon.getPosition().x);
            }
        }

        void moveBullets(sf::RenderWindow &window, float elapsed_time) {
            for (int i = 0; i < max_bullets; i++) {
                if (active_bullets[i]) {
                    // bullet is active and needs to move
                    window.draw(bullets[i]);
                    bullets[i].move(elapsed_time * bullet_velocity * std::cos(bullet_angles[i]),
                                    elapsed_time * bullet_velocity * std::sin(bullet_angles[i]));
                }
            }
        }

        bool checkCollision(Ship ship, int max_x, int max_y) {
            for (int i = 0; i < max_bullets; i++) {
                int bullet_posx = bullets[i].getPosition().x;
                int bullet_posy = bullets[i].getPosition().y;
                if (active_bullets[i]) {
                    if (ship.ship_sprite.getGlobalBounds().contains(bullet_posx, bullet_posy)) {
                        deactivateBullet(i, max_x, max_y);
                        return true;
                    }
                }
            }
            return false;
        }

        void checkBulletBoundaries(int max_x, int max_y) {
            for (int i = 0; i < max_bullets; i++) {
                int bullet_posx = bullets[i].getPosition().x;
                int bullet_posy = bullets[i].getPosition().y;
                if (bullet_posx > max_x || bullet_posx < 0) {
                    deactivateBullet(i, max_x, max_y);
                } else if (bullet_posy > max_y || bullet_posy < 0) {
                    deactivateBullet(i, max_x, max_y);
                }
            }
        }

        void deactivateBullet(int idx, int max_x, int max_y) {
            active_bullets[idx] = 0;
            bullet_angles[idx] = 0.f;
            bullets[idx].setPosition(max_x / 2, max_y - 30);
        }
};

void gameLoop(sf::RenderWindow &window, 
              int boat_time, int max_bullets,
              int max_x, int max_y) {
    // game components
    Aim cursor = Aim();
    Water water = Water();
    std::vector<Ship> ships;
    //int boat_time = boat_spawn;
    //int max_bullets = bullet_count;
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
        cursor.aim.setPosition(mouse_posx, mouse_posy);

        //draw elements
        window.clear();
        window.draw(water.background);
        window.draw(cursor.aim);
        window.draw(cannon.cannon);

        // handle ships
        std::vector<Ship>::iterator it;        
        for (it = ships.begin(); it < ships.end(); it++) {
            window.draw(it->ship_sprite);
            // update ship position
            it->ship_sprite.move(elapsed_time * 100.0, 0);
            if (it->ship_sprite.getPosition().x > max_x) {
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
