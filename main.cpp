#include "elevator.h"
#include <iostream>
#include <limits>

int main(int argc, char* argv[]) {
    elevator *elevator1 = new elevator(1, 1), *elevator2 = new elevator(2, 1);
    int curr, dest;
    while(true) {
        std::cout << "Enter your floor:";
        while (!(std::cin >> curr)) { // If input conversion fails (e.g., due to non-numeric input)
            std::cin.clear(); // Clear the failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter your floor:";
        }
        std::cout << "Enter the floor you want to go to:";
        while (!(std::cin >> dest)) { // If input conversion fails (e.g., due to non-numeric input)
            std::cin.clear(); // Clear the failbit
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter the floor you want to go to:";
        }
        if(abs(elevator1->get_current_floor()-curr) > abs(elevator2->get_current_floor()-curr))
            elevator2->move(curr, dest);
        else
            elevator1->move(curr, dest);
            
    }
}