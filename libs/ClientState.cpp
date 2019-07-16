#include "States/ClientState.hpp"

namespace cp
{
    ClientState::ClientState(GameDataRef _data, Server_ptr server, int unique_id) 
    : game_data(_data), simulator(_data), server(server), my_id(unique_id) {}
    ClientState::~ClientState() {}

    void ClientState::init() {
        simulator.init();
        std::cout << "client id: " << my_id << std::endl;
        simulator.add_external_player(my_id);
        simulator.update_main_player(my_id);
    }

    void ClientState::handle_input(float delta) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            game_data->machine.remove_state();
        }
        //if (server->getLastStatus() == sf::Socket::Disconnected) {
        //    game_data->machine.remove_state();
        //}
        game_data->input.register_input(simulator.get_input());
        simulator.handle_input(delta);
        //collect_inputs();
        //use_collected_inputs();
    }

    void ClientState::update(float delta) {
        //get_network_snap();
        //use_network_snap();
        simulator.update(delta);
    }

    void ClientState::draw(float delta) {
        simulator.draw(delta);
    }

    void ClientState::collect_inputs() {
        input_to_send = simulator.get_input();
    }

    void ClientState::use_collected_inputs() {
        (*server) << input_to_send;
    }

    void ClientState::get_network_snap() {
        (*server) >> snap;
    }

    void ClientState::use_network_snap() {
        simulator.use_snap(snap, true);
    }

}
