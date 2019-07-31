/**/

#include "DEFINITIONS.hpp"
#include <iostream>
#include <time.h>
#include "Game.hpp"
#include "Sensor.hpp"

Sensor* G_sensor;

int main(int argc, char* argv[])
{
    G_sensor = new Sensor();
    if (argc > 1) {
        std::cout << argv[0] << " " << argv[1] << std::endl;
        G_sensor->SerialInit(argv[1]);
    }
    else {
        std::cout << argv[0] << std::endl;
    }

    srand(static_cast<unsigned int>(time(NULL))); // set seed value for rand()
    cp::Game();
    delete G_sensor;
    return EXIT_SUCCESS;
}

/**/
