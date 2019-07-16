/**
 * @file Client.hpp
 * @author Vijay
 * @brief Client class refers to a another pc connected over network
 * @version 0.1
 * @date 2019-02-26
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "States/GameSimulator.hpp"
#include "SFML/Network.hpp"

namespace cp
{
    class Client
    {
    public:
        using ID = long long int;
        using IP = std::string;
        using key_input_type = std::pair<ID, std::vector<bool>>;

        Client(ID identity) : id(identity) {}
        ID get_identity() const { return id; }
        sf::TcpSocket& get_socket() { return socket; }

        friend Client& operator << (Client& client, const GameSimulatorSnap& snap) {
            sf::Packet packet;
            packet << snap;
            client.send_packet(packet);
            return client;
        }

        friend Client& operator >> (Client& client, key_input_type& labelled_input) {
            sf::Uint64 size;
            bool res;
            sf::Packet packet;
            client.recieve_packet(packet);
            if (client.getLastStatus() == sf::Socket::Done) {
                packet >> labelled_input.first >> size;
                for (int i = 0; i < size; i++) {
                    packet >> res;
                    labelled_input.second.push_back(res);
                }
            }
            return client;
        }

        void connect_to(const std::string & ip, int port) {
            while ((last_status = socket.connect(ip, port, sf::seconds(5))) == sf::Socket::Partial) {
                std::cout << "try to connect: " << ip << ":" << port << std::endl;
            }
        }

        void send_packet(sf::Packet& packet) {
            while ((last_status = socket.send(packet)) == sf::Socket::Partial) {
                std::cout << "partially sent" << std::endl;
            }
        }

        void recieve_packet(sf::Packet& packet) {
            while ((last_status = socket.receive(packet)) == sf::Socket::Partial) {
                std::cout << "partially recieved" << std::endl;
            }
        }

        sf::Socket::Status getLastStatus() const {
            return last_status;
        }

    private:
        ID id;
        sf::TcpSocket socket;
        sf::Socket::Status last_status = sf::Socket::Status::NotReady;
    };
}

#endif // CLIENT_HPP
