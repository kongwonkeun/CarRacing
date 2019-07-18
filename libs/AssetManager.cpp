/**/

#include "ResourceManagers/AssetManager.hpp"
#include <iostream>

namespace cp
{
    AssetManager::AssetManager() {}
    AssetManager::~AssetManager() {}

    void AssetManager::load_texture(std::string name, std::string file_name) {
        sf::Texture texture;
        if (texture.loadFromFile(file_name))
            map_texture[name] = texture;
        else
            std::cout << "fail to load texture" << std::endl;
    }

    void AssetManager::load_font(std::string name, std::string file_name) {
        sf::Font font;
        if (font.loadFromFile(file_name))
            map_font[name] = font;
        else
            std::cout << "fail to load font" << std::endl;
    }

    sf::Texture& AssetManager::get_texture(std::string name) {
        return map_texture.at(name);
    }

    sf::Font& AssetManager::get_font(std::string name) {
        return map_font.at(name);
    }

}

/**/
