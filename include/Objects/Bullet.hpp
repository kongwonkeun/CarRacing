#ifndef BULLET_HPP
#define BULLET_HPP

#include <vector>
#include "Car.hpp"
#include "Camera.hpp"

namespace cp
{
    class Bullet : public Car
    {
    public:
        Bullet(GameDataRef _data, int car_num);
        ~Bullet();
        virtual void init(sf::Vector3f pos);
        virtual void update_car(float dt, const std::vector<Line>& lines, float segL);
        void drawSprite(const Line& line);
        void handle_input();

        int frames = 0;
    };
}

#endif // BULLET_HPP
