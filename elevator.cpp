#include "elevator.h"
#include <iostream>
#include <chrono>
#include <thread>

elevator::elevator(int number, int top, int btm)
{
    elevator::top_floor = top;
    elevator::btm_floor = btm;
    elevator::elvt_no = number;
    elevator::current_floor = btm;
    std::thread elvt_thread(&elevator::run, this);
    elvt_thread.detach();
}
void elevator::run()
{
    while(true) {
        while(elevator::up_floors.size()) {
            elevator::state = 1;
            while(elevator::current_floor != elevator::up_floors.top()) {
                DELAY_SEC(1);
                if(elevator::current_floor < elevator::up_floors.top())
                    elevator::current_floor++;
                else
                    elevator::current_floor--;
            }
            elevator::up_floors.pop();
        }
        while(elevator::down_floors.size()) {
            elevator::state = 2;
            while(elevator::current_floor != elevator::down_floors.top()) {
                DELAY_SEC(1);
                if(elevator::current_floor > elevator::down_floors.top())
                    elevator::current_floor--;
                else
                    elevator::current_floor++;
            }
            elevator::down_floors.pop();
        }
        elevator::state = 0;
    }
}
void elevator::display_floor()
{
    std::cout << "Elevator [" << elevator::get_elvt_no() << "] is on the floor " << elevator::get_floor();
    int state = elevator::get_state();
    if(state == 0)
        std::cout << " - IDLE" << std::endl;
    else if(state == 1)
        std::cout << " - UP" << std::endl;
    else
        std::cout << " - DOWN" << std::endl;
}

void elevator::move(int current, int floor)
{
    if(elevator::get_state() == 0) {
        if(floor < current) {
            elevator::down_floors.push(current);
            elevator::down_floors.push(floor);
        }
        else {
            elevator::up_floors.push(current);
            elevator::up_floors.push(floor);
        }
    }
    else if(elevator::get_floor() > current) {
        elevator::down_floors.push(current);
        elevator::down_floors.push(floor);
    }
    else {
        elevator::up_floors.push(current);
        elevator::up_floors.push(floor);
    }
}

int elevator::get_elvt_no()
{
    return elevator::elvt_no;
}

int elevator::get_floor()
{
    return elevator::current_floor;
}

int elevator::get_state()
{
    return elevator::state;
}
