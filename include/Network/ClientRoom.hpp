#ifndef CLIENT_ROOM_HPP
#define CLIENT_ROOM_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include "States/State.hpp"
#include "Network/Server.hpp"
#include "States/ClientState.hpp"
#include "Game.hpp"
#include "Type.hpp"

namespace cp
{
    class ClientRoom : public State
    {
    public:
        using ID = int;
        using Server_ptr = std::shared_ptr<Server>;

        ClientRoom(GameDataRef _data) : game_data(_data) {}
        ~ClientRoom() {}

        void init() {
            server_ptr = Server_ptr(new Server(server_id));
            enterIP();

            server_ptr->connect_to(HOST_IP, PORT);
            sf::Socket::Status status = server_ptr->getLastStatus();
            if (status != sf::Socket::Done) {
                std::cout << "server connection error" << std::endl;
                game_data->machine.remove_state();
                update_required = false;
            }
            else {
                sf::Packet packet;
                (*server_ptr).get_socket().receive(packet);
                packet >> unique_id;
                std::cout << "server connected and player id received: " << unique_id << std::endl;
            }
        }

        void handle_input(float delta) {
            if (!update_required) return;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                game_data->machine.remove_state();
                update_required = false;
            }
        }

        void update(float delta) {
            if (!update_required) return;
            get_notifications();
            use_notification();
        }

        void draw(float delta) {}

        void enterIP() {
            background_sprite.setTexture(game_data->assets.get_texture("MainMenuStateBackground"));
            background_sprite.scale(1, 600.0f / 512.0f);
            background_sprite.setPosition(
                SCREEN_WIDTH  / 2.0f - background_sprite.getGlobalBounds().width  / 2.0f,
                SCREEN_HEIGHT / 2.1f - background_sprite.getGlobalBounds().height / 2.0f
            );

            sf::RenderWindow ipadd(sf::VideoMode(500, 150), "Enter IP Address");
            sf::Text text1, text2;
            sf::Font font;
            font.loadFromFile("../res/SFAtarianSystem.ttf");
            text1.setFont(font);
            text1.setString("ENTER HOST IP ADDRESS");
            text1.setPosition(75, 40);
            text1.setCharacterSize(24);
            text1.setStyle(sf::Text::Bold);
            text1.setOutlineThickness(2);
            text2.setFont(font);
            text2.setPosition(75, 80);
            text2.setCharacterSize(24);
            text2.setStyle(sf::Text::Bold);
            text2.setOutlineThickness(2);

            while (ipadd.isOpen()) {
                game_data->window.clear();
                game_data->window.draw(background_sprite);
                game_data->window.display();

                ipadd.clear(sf::Color(0, 0, 0, 255));
                ipadd.draw(text1);
                ipadd.draw(text2);
                sf::Event event;

                while (ipadd.pollEvent(event)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                        HOST_IP = text2.getString();
                        ipadd.close();
                    }
                    switch (event.type) {
                        case sf::Event::Closed:
                            ipadd.close();
                            break;
                        case sf::Event::TextEntered:
                            text2.setString(text2.getString() + event.text.unicode);
                            break;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
                        text2.setString(text2.getString().substring(0, text2.getString().getSize() - 1));
                        continue;
                    }
                }
                ipadd.display();
            }
        }

        void get_notifications() {
            sf::Packet packet;
            server_ptr->recieve_packet(packet);
            if (server_ptr->getLastStatus() != sf::Socket::Done) {
                HAS_TO_WAIT = 2;
            }
            else {
                packet >> HAS_TO_WAIT;
            }
        }

        void use_notification() {
            if (HAS_TO_WAIT == 1) return;
            if (HAS_TO_WAIT == 0) {
                std::cout << "server is ready to start playing" << std::endl;
                game_data->machine.add_state(StateRef(new ClientState(game_data, server_ptr, unique_id)), true);
                update_required = false;
            }
            else if (HAS_TO_WAIT == 2) {
                game_data->machine.remove_state();
                update_required = false;
            }
        }

    private:
        std::string HOST_IP;
        GameDataRef game_data;
        Server_ptr server_ptr;
        sf::Sprite background_sprite;
        int HAS_TO_WAIT = 0;
        bool update_required = true;
        ID server_id = ID_HOST_PLAYER;
        ID unique_id = 0;
    };
}

#endif // CLIENT_ROOM_HPP
