#include "Objects/Car.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

namespace cp
{
    Car::Car(GameDataRef _data, int car_num, ID id) : data(_data), car_image_num(car_num), id(id), in_use(false) {
        sprite.setTexture(data->assets.get_texture("CarImage" + std::to_string(car_image_num)));
        sprite.setPosition(
            SCREEN_WIDTH  / 2.0f - sprite.getGlobalBounds().width  / 2.0f, 
            SCREEN_HEIGHT / 1.0f - sprite.getGlobalBounds().height * 1.5f
        );
        e_position = sf::Vector3f(0, 0, 0);
        e_speed = sf::Vector3f(0, 0, 0);
        float max_speed = 400;
        e_max_speed    = sf::Vector3f(0, 0,  max_speed);
        e_acceleration = sf::Vector3f(0, 0,  max_speed / 5);
        e_decleration  = sf::Vector3f(0, 0, -max_speed / 5);
    }

    Car::~Car() {
    }

    void Car::draw_car() {
        if (!in_use) return;
        data->window.draw(sprite);
    }

    void Car::onCollision() {
        if (health > 0) health -= 1;
        else if (health == 0) in_use = false;
        else health = 0;
        e_speed.z = std::max(-50.0f, std::min(e_speed.z, e_max_speed.z));
    }

}
