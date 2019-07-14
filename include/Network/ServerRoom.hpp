/**
 * @file ServerRoom.hpp
 * @author Vijay
 * @brief Simple Server room displayed before Online Play Starts
 * @version 0.1
 * @date 2019-02-27
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef SERVER_ROOM_HPP
#define SERVER_ROOM_HPP

#include "States/State.hpp"
#include "Game.hpp"
#include "Network/Client.hpp"
#include <iostream>
#include <cstring>
#include "States/ServerState.hpp"
#include <set>
#include <fstream>
#include <memory>

namespace cp
{
    class ServerRoom : public State
    {
    public:
        using TcpClient_ptr = std::shared_ptr<Client>;

        ServerRoom(GameDataRef _data) : game_data(_data) {}
        ~ServerRoom() {}

        void init() {
            // TODO : initialize and use gui.
            clock.restart();
            std::cout << "Opening Port ----1111----" << std::endl;
            int n_try = 0;
            while (listener.listen(PORT) != sf::Socket::Done && n_try < 10) {
                std::cout << "Error Listening Port" << std::endl;
                n_try++;
            }
            if (n_try >= 10) {
                std::cout << "Error Time Over" << std::endl;
                PORT_OPEN_SUCCESS = false;
                handle_port_problem();
                return;
            }
            std::cout << "Port Opened" << std::endl;
            selector.add(listener);
        }

        virtual void handle_input(float delta) {
            if (!update_required) return;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                JUST_WAIT = 2;
                notify_clients();
                std::cout << "Server host closed" << std::endl;
                game_data->machine.remove_state();
                update_required = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                std::cout << "Delibirately started" << std::endl;
                JUST_WAIT = 0;
                notify_clients();
                game_data->machine.add_state(StateRef(new ServerState(game_data, clients_res)), true);
                for (auto it : clients_res) it->get_socket().setBlocking(false);
                update_required = false;
            }
        }

        virtual void update(float delta) {
            if (!update_required) return;
            if (clock.getElapsedTime().asSeconds() > WAITING_ROOM_TIME) {
                JUST_WAIT = 0;
                update_required = false;
                std::cout << "Wait time exceeded" << std::endl;
                game_data->machine.add_state(StateRef(new ServerState(game_data, clients_res)), true);
            }
            handle_dead_clients();
            notify_clients();
            check_incoming_connections();
        }

        virtual void draw(float delta) {}
        virtual void pause() {}
        virtual void resume() {}

    private:
        void check_incoming_connections() {
            if (unassigned_id.size() == 0) return;
            if (selector.wait(sf::milliseconds(1))) {
                if (selector.isReady(listener)) {
                    int unique_id = *unassigned_id.begin();
                    TcpClient_ptr client = TcpClient_ptr(new Client(unique_id));
                    if (listener.accept(client->get_socket()) == sf::Socket::Done) {
                        clients_res.insert(client);
                        sf::Packet packet; 
                        packet << unique_id;
                        client->get_socket().send(packet);
                        unassigned_id.erase(unique_id);
                        std::cout << "One connected with id:" << unique_id << std::endl;
                    }
                }
            }
        }

        void handle_port_problem() {
            if (PORT_OPEN_SUCCESS == false) {
                game_data->machine.remove_state();
                update_required = false;
            }
        }

        void handle_dead_clients() {
            //for (auto it = clients_res.begin(); it != clients_res.end();) {
            //    if ((*it)->get_socket().Disconnected) {
            //        std::cout << "Removing id with " << (*it)->get_identity() << std::endl;
            //        unassigned_id.insert((*it)->get_identity());
            //        it = clients_res.erase(it);
            //    } else {
            //        ++it;
            //    }
            //}
        }

        void notify_clients() {
            sf::Packet response;
            response << JUST_WAIT;
            for (auto client_ptr : clients_res) {
                std::cout << JUST_WAIT;
                client_ptr->send_packet(response);
            }
        }

        sf::Sprite background_sprite;
        GameDataRef game_data;
        std::set<TcpClient_ptr> clients_res;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        std::set<int> unassigned_id = { 123, 12331, 214231, 2332 };
        bool PORT_OPEN_SUCCESS = true;
        int JUST_WAIT = 1;
        sf::Clock clock;
        bool time_over = false;
        bool update_required = true;
    };
}

#endif // SERVER_ROOM_HPP
