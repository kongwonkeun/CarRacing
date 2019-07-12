#include "DEFINITIONS.hpp"
#include <time.h>
#include "Game.hpp"
#include <iostream>

int main()
{
    srand(static_cast<unsigned int>(time(NULL))); // set seed value for rand()
    cp::Game();
    return EXIT_SUCCESS;
}
