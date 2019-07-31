#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <iostream>
#include "SFML/Graphics.hpp"

#define SPLASH_STATE_SHOW_TIME 1.0
#define SPLASH_SCENE_BACKGROUND_FILEPATH "../res/splashScreen.png"
#define MAIN_MENU_BACKGROUND_FILEPATH "../res/splashScreen.png"
#define GAME_BACKGROUND_FILEPATH "../res/sky.png"
#define GAME_STATE_BACKGROUND_FILEPATH "../res/cy.png"
#define GAME_OVER_BACKGROUND_FILEPATH "../res/busted.png"
#define GAME_TITLE_FILEPATH "../res/title.png"
#define RESUME_BUTTON_FILEPATH "../res/PlayButton3.png"
#define HOST_PLAY_BUTTON_FILEPATH "../res/PlayButton2_host.png"
#define JOIN_PLAY_BUTTON_FILEPATH "../res/PlayButton2_join.png"
#define SINGLE_PLAY_BUTTON_FILEPATH "../res/PlayButton1.png"
#define MENU_BUTTON_FILEPATH "../res/MenuButton.png"
static std::string CAR_NAME(int x)   { return("../res/CarImage"  + std::to_string(x) + ".png"); }
static std::string P_CAR_NAME(int x) { return("../res/PCarImage" + std::to_string(x) + ".png"); }
static std::string FIRE_NAME(int x)  { return("../res/f" + std::to_string(x) + ".png"); }
#define CAR_IMAGE_FILEPATH(i)   CAR_NAME(i)
#define P_CAR_IMAGE_FILEPATH(i) P_CAR_NAME(i)
#define FIRE_IMAGE_FILEPATH(i)  FIRE_NAME(i)
//#define SCREEN_WIDTH  1024
//#define SCREEN_HEIGHT 768
const int SCREEN_WIDTH  = sf::VideoMode::getDesktopMode().width;
const int SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height;
//#define ROAD_WIDTH 2000
#define ROAD_WIDTH 4000
#define SEGMENT_LENGTH 200
#define CAMERA_DEPTH 0.84f
#define CAMERA_POSITION_Y 1500
#define CAR_HEALTH 100
#define CAR_MASS 100
#define CAR_CENTRIFUGAL 0.5f
#define CAR_MAX_SPEED 600

#define GAME_DELTA (1.0f / 60.f)

#define PORT 20000
//#define WAITING_ROOM_TIME 100000
#define WAITING_ROOM_TIME 30
#define MAX_EXTERNAL_PLAYER 4
#define ID_HOST_PLAYER 1
#define ID_JOIN_PLAYER 2
#define ID_BOT_1 -1
#define ID_BOT_2 -2
#define TOTAL_BOTS 2
#define TOTAL_CAR_IMAGES 10
#define COEFFICIENT_OF_RESTITUTION 0.8f
#define MAX_ENTITIES 1000

const sf::Vector2f Scaling_factor(1368.0f / 1920.0f, 768.0f / 1080.0f);

#endif // DEFINITIONS_HPP
