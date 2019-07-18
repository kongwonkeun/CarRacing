#ifndef CARS_HPP
#define CARS_HPP

#include <iostream>
#include "Car.hpp"
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Camera.hpp"

namespace cp
{
    class PlayerCar : public Car
    {
    public:
        PlayerCar(GameDataRef _data, int _car_num, ID id);
        ~PlayerCar();
        void handle_input(std::vector<bool> mask, float dt);
        void update_car(float dt, const std::vector<Line>& lines, float segL);
        void drawSprite(const Line& line);
    
        float friction = e_max_speed.z / 5;
        int img = 1;
    };
}

#endif // CARS_HPP
