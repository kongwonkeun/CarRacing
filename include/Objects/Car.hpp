#ifndef CAR_HPP
#define CAR_HPP

#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include "Line.hpp"
#include "Type.hpp"

namespace cp
{
    class Car
    {
    public:
        Car(GameDataRef _data, int car_num, ID id);
        ~Car();
        virtual void init(sf::Vector3f pos) {}
        virtual void update_car(float dt, const std::vector<Line>& lines, float segL) = 0;
        void draw_car();
        void reset() { in_use = false; }
        void onCollision();
        sf::Vector3f getPosition() const { return e_position; }
        sf::Vector3f getSpeed() const { return e_speed; }

        GameDataRef data;
        sf::Sprite sprite;
        sf::Vector3f e_position;
        sf::Vector3f e_speed;
        sf::Vector3f e_acceleration;
        sf::Vector3f e_decleration;
        sf::Vector3f e_max_speed;
        bool  in_use = false;
        float centrifugal = CAR_CENTRIFUGAL;
        float car_mass = CAR_MASS;
        float health = CAR_HEALTH;
        int car_image_num;
        ID id = 0;
    };
}

#endif // CAR_HPP
