#include "States/ServerState.hpp"
#include "Network/ServerRoom.hpp"

namespace cp
{
    ServerState::ServerState(GameDataRef _data, std::set<TcpClient_ptr> clients) 
    : game_data(_data), simulator(_data), clients(std::move(clients)) {}
    ServerState::~ServerState() {}

    void ServerState::init() {
        simulator.init();
        simulator.add_external_player(ID_HOST_PLAYER);
        simulator.update_main_player(ID_HOST_PLAYER);
        for (auto& client : clients) {
            std::cout << "client id: " << client->get_identity() << std::endl;
            simulator.add_external_player(client->get_identity());
            simulator.ext_inp_reg();
            client->get_socket().setBlocking(false);
        }
    }

    void ServerState::handle_input(float delta) {
        if (!update_required) return;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            game_data->machine.remove_state();
            update_required = false;
        }
        collect_network_inputs();
        use_collected_inputs();
        game_data->input.register_input(simulator.get_input());
        simulator.handle_input(delta);
    }

    void ServerState::update(float delta) {
        if (!update_required) return;
        simulator.update(delta);
        generate_outputs();
        use_generated_outputs();
    }

    void ServerState::draw(float delta) {
        simulator.draw(delta);
    }

    void ServerState::collect_network_inputs() {
        inputs.clear();
        for (auto& client : clients) {
            Client::key_input_type temp;
            (*client) >> temp;
            inputs.push_back(std::move(temp));
        }
    }

    void ServerState::use_collected_inputs() {
        for (auto& input : inputs) {
            game_data->input.register_input(input);
        }
    }

    void ServerState::generate_outputs() {
        temp_snap = simulator.get_current_snap(SnapFlag::NETWORK_SNAP);
    }

    void ServerState::use_generated_outputs() {
        for (auto& client : clients) {
            (*client) << temp_snap;
        }
    }

}
