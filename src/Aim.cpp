#include "../include/Aim.h"

Aim::Aim() {
    aim = sf::CircleShape(5.f);
    aim.setFillColor(sf::Color::White);
    aim.setOrigin(10, 10);
}

sf::CircleShape Aim::getAim() {
    return aim;
}

void Aim::setAim(int pos_x, int pos_y) {
    aim.setPosition(pos_x, pos_y);
}
