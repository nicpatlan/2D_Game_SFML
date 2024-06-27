#ifndef _SHIP_H_
#define _SHIP_H_

class Ship {
    private:
        sf::Texture ship_texture;
        sf::Sprite ship_sprite;
    public:
        Ship(int ship_posy, int ship_posx = 0) {
            if (!ship_texture.loadFromFile("src/assets/redShip.png")) {
                std::cout << "error loading sprite" << std::endl;
            }
            ship_sprite.setTexture(ship_texture);
            ship_sprite.setPosition(sf::Vector2f(0, ship_posy));
            ship_sprite.scale(sf::Vector2f(0.03, 0.03));
        }

        sf::Sprite getSprite() {
            return ship_sprite;
        }

        void moveSprite(int move_posx) {
            ship_sprite.move(move_posx, 0);
        }
};
#endif // _SHIP_H_
