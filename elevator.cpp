#include "elevator.h"
#include <iostream>
#include <chrono>
#include <thread>

#define delay(x) std::this_thread::sleep_for(std::chrono::seconds(x))

void broadcast() {
    std::cout << "\t(Door is open)" << std::endl;
    delay(1);
    std::cout << "\t(Door is close)" << std::endl;
}
elevator::elevator(int number, int floor)
{
    elevator::number = number;
    elevator::current_floor = floor;
}
void elevator::display_floor()
{
    std::cout << "The elevator " << elevator::number << " is currently on the floor " << elevator::current_floor << std::endl;
}

void elevator::move(int current, int floor)
{
    move2floor(current);
    elevator::display_floor();
    broadcast();
    if(current > floor)
        std::cout << "\t(Going down)" << std::endl;
    else if(current < floor)
        std::cout << "\t(Going up)" << std::endl;
    move2floor(floor);
    elevator::display_floor();
    broadcast();
}

void elevator::move2floor(int floor) {
    while(elevator::current_floor > floor) {
        elevator::display_floor();
        delay(1);
        elevator::current_floor--;
    }
    while(elevator::current_floor < floor) {
        elevator::display_floor();
        delay(1);
        elevator::current_floor++;
    }
}

int elevator::get_current_floor()
{
    return elevator::current_floor;
}
