#include "Objects/Bullet.hpp"
#include "DEFINITIONS.hpp"
#include <iostream>
#include <cmath>
#include <sstream>

namespace cp
{
    Bullet::Bullet(GameDataRef _data, int car_num): Car(_data, car_num) {
        e_speed = sf::Vector3f(0, 0, 600);
        e_max_speed = sf::Vector3f(0, 0, 600);
        health = 0.1f;
        sprite.setTexture(data->assets.get_texture("Bullet"));
        sprite.scale(0.01f, 0.01f);
    }

    Bullet::~Bullet() {
    }

    void Bullet::init(sf::Vector3f pos) {
        e_speed = sf::Vector3f(0, 0, 600);
        e_position = pos;
        frames = 0;
        in_use = true;
    }

    void Bullet::drawSprite(const Line& line) {
        if (!in_use) return;
        sf::Sprite& s = sprite;
        int w = s.getTextureRect().width;
        int h = s.getTextureRect().height;
        float destX = line.no_curve_X + line.scale * e_position.x * SCREEN_WIDTH / 2;
        float destY = line.no_curve_Y + 4;
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

    void Bullet::update_car(const float dt, const std::vector<Line>& lines, const float segL) {
        frames++;
        if (frames > 30) in_use = false;
        if (!in_use) return;
        e_position.z += e_speed.z;
    }

    void Bullet::handle_input() {
    }

}
