#ifndef _WATER_H_
#define _WATER_H_

class Water {
    private:
        sf::Texture back_texture;
        sf::Sprite background;
    public:
        Water() {
            if (!back_texture.loadFromFile("src/assets/blueWater.png")) {
                std::cout << "error loading background" << std::endl;
            }
            background.setTexture(back_texture);
            background.scale(sf::Vector2f(5, 3));
        }

        sf::Sprite getBackground() {
            return background;
        }
};
#endif //_WATER_H_
