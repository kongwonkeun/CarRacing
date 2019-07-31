/**/

#include <iostream>
#include <cmath>
#include <time.h>
#include <sstream>
#include "Objects/PlayerCar.hpp"
#include "DEFINITIONS.hpp"
#include "fstream"
#include "Sensor.hpp"

namespace cp
{
    PlayerCar::PlayerCar(GameDataRef _data, int car_num, ID id) : Car(_data, car_num, id) {
        sprite.setTexture(data->assets.get_texture("CarImage" + std::to_string(car_image_num)));
        e_position.x = static_cast<float>(-2 + rand() % 4);
        e_position.z = static_cast<float>(10000 + rand() % 10000);
        e_speed = sf::Vector3f(0.0f, 0.0f, static_cast<float>(rand() % 20));
        if (car_image_num == 8) {
            health = 50;
            e_max_speed.z = 250;
        }
        car_mass = 100;
        in_use = true;
    }

    PlayerCar::~PlayerCar() {
    }

    void PlayerCar::handle_input(input_type mask, float dt) {
        float speedRatio = std::abs(e_speed.z / e_max_speed.z);
        float dx = 5 * dt * speedRatio;

        //---- kong ---- need to change for k-roller
        if      (mask[0]) {
            if (G_sensor->in_use &&  mask[0] > 1) {
                e_speed.z  = (float)(mask[0] / 2);
            }
            else {
                e_speed.z += (e_speed.z == 0) ? 15 : (e_acceleration.z * dt);
            }
        }
        else if (mask[1]) { e_speed.z += (e_speed.z > 15 || e_speed.z < -15 ) ? e_decleration.z * dt : (e_speed.z > 0 ? -e_speed.z : -15); }
        if      (mask[2]) { e_position.x -= dx; }
        else if (mask[3]) { e_position.x += dx; }
        //----
        
        e_speed.z   -= std::abs(e_speed.z) < 15 ? e_speed.z : (e_speed.z / 250);
        e_speed.z    = std::max(-100.0f, std::min(e_speed.z, e_max_speed.z));
        e_position  += e_speed;
        e_position.x = std::max(-4.0f, std::min(e_position.x, 3.0f));

        if (health <= 0 and car_image_num == 8) {
            sprite.setTexture(data->assets.get_texture("f" + std::to_string((int)std::ceil(((float)(img++)) / 4))));
            if (img >= 48) {
                *this = PlayerCar(data, this->car_image_num, 0);
                img = 1;
            }
        }
        if (std::abs(e_speed.z) > 0.1) {
            if (mask[2])      sprite.setTexture(data->assets.get_texture("CarImage" + std::to_string(car_image_num - 2)));
            else if (mask[3]) sprite.setTexture(data->assets.get_texture("CarImage" + std::to_string(car_image_num - 1)));
            else              sprite.setTexture(data->assets.get_texture("CarImage" + std::to_string(car_image_num)));
        }
    }

    void PlayerCar::update_car(const float dt, const std::vector<Line>& lines, const float segL) {
        float speedRatio = std::abs(e_speed.z / e_max_speed.z);
        float dx = 3 * dt * speedRatio;
        Line index_line = lines[static_cast<__int64>(e_position.z / segL)];
        e_position.x -= (dx * speedRatio * index_line.curve * centrifugal);
        e_position.x = std::max(-1.0f, std::min(e_position.x, 1.0f));
    }

    void PlayerCar::drawSprite(const Line& line) {
        sf::Sprite& s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;
        float destX = line.X + line.scale * e_position.x * SCREEN_WIDTH / 2;
        float destY = line.Y + 4;
        float destW = w * line.W / 266;
        float destH = h * line.W / 266;
        destX += destW * e_position.x; // offsetX
        destY -= destH; // offsetY

        float clipH = destY + destH - line.clip;
        if (clipH < 0) clipH = 0;
        if (clipH >= destH) return;

        s.setScale(destW / w, destH / h);
        s.setPosition(destX, destY);
        data->window.draw(s);
    }

}

/**/
