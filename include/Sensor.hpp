#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <iostream>
#include <string>
#include <Windows.h>

class Sensor
{
public:
    Sensor();
    ~Sensor();
    void SerialInit(char *port);
    static void SerialReadThread(void* myInstant);
    void SerialRead();
    void SendScore(long long int score);

    bool in_use = false;
    int  direction = 0;
    int  speed = 0;

private:
    void ReadStateMachine(int b);

    HANDLE _score = 0;
    HANDLE _handle = 0;
    HANDLE _thread = 0;
    DWORD _id = 0;
    bool _run = false;
    int _s = 0;
    int _v = 0;
    int _d = 0;
    int _d_last = 0;
};
extern Sensor* G_sensor;

#endif // SENSOR_HPP
