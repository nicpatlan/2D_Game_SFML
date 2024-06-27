#ifndef _AIM_H_
#define _AIM_H_

class Aim {
    private:
        sf::CircleShape aim;
    public:
        Aim() {
            aim = sf::CircleShape(5.f);
            aim.setFillColor(sf::Color::White);
            aim.setOrigin(10, 10);
        }

        sf::CircleShape getAim() {
            return aim;
        }

        void setAim(int pos_x, int pos_y) {
            aim.setPosition(pos_x, pos_y);
        }
};
#endif //_AIM_H_
