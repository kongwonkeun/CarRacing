#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Line.hpp"
#include <SFML/Graphics.hpp>
#include "Car.hpp"
#include <thread>

namespace cp
{
    class Camera
    {
    public:
        void handle_input() {
            if      (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) { e_position.z += 200; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) { e_position.z -= 200; }
            if      (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) { e_position.x -= 100; }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) { e_position.x += 100; }
        }
        void catch_player(const Car& player) {
            e_speed = player.getSpeed();
        }
        const sf::Vector3f& getPosition() const { return e_position; }
        const sf::Vector3f& getSpeed() const { return e_speed; }
        float getCamD() const { return camD; }

        std::thread::id id;
        sf::Vector3f e_position = sf::Vector3f(0, CAMERA_POSITION_Y, 0);

    private:
        sf::Vector3f e_speed;
        float camD = CAMERA_DEPTH;
    };
}

#endif // CAMERA_HPP
