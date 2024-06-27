#ifndef _CANNON_H_
#define _CANNON_H_

class Cannon {
    private:
        sf::Texture cannon_texture;
        sf::Sprite cannon;
        int max_bullets;
        int bullet_idx;
        float bullet_velocity;
        std::vector<sf::CircleShape> bullets;
        std::vector<float> bullet_angles;
        std::vector<int> active_bullets;
    public:
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

        sf::Sprite getCannon() {
            return cannon;
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
                    if (ship.getSprite().getGlobalBounds().contains(bullet_posx, bullet_posy)) {
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
#endif // _CANNON_H_
