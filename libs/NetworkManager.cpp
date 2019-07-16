#include "NetworkManager.hpp"
#include <cstring>

sf::TcpSocket cp::NetworkManager::client;

namespace cp
{
    NetworkManager::NetworkManager() {}
    NetworkManager::~NetworkManager() {}

    void NetworkManager::createServer() {
        sf::TcpListener listener;
        if (listener.listen(53000) != sf::Socket::Done) {
            std::cerr << "port listen error" << std::endl;
        }
        if (listener.accept(NetworkManager::client) != sf::Socket::Done) {
            std::cerr << "client accept error" << std::endl;
        } else {
            char data[100] = "nicemeetyou";
            if (client.send(data, strlen(data)) != sf::Socket::Done) {
                std::cerr << "data transfer error" << std::endl;
            }
        }
    }

    void NetworkManager::createClient() {
        run(0);
    }

    void NetworkManager::run(int type) {
    }

    void NetworkManager::sendData(sf::Vector3f pos) {
        std::thread senderThread(send, pos);
        senderThread.join();
    }

    void NetworkManager::send(sf::Vector3f pos) {
        sf::Packet packet;
        packet << pos.x << pos.y << pos.z;
        if (client.send(packet) != sf::Socket::Done) {
            std::cerr << "data transfer error" << std::endl;
        }
    }

}
