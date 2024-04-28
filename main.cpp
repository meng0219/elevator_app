#include "elevator.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>

#define pii std::pair<int, int>
#define PORT 8080

std::mutex screenMutex;

void print_prompt(std::string prompt, int x, int y) {
    screenMutex.lock();
        MVCURSOR(x, y);
        CLEAR_DOWNSCREEN;
        std::cout << prompt;
    screenMutex.unlock();
}
void get_int_input(int &val, std::string prompt, int x, int y) {
    print_prompt(prompt, x, y);
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        print_prompt("Invalid input. Please enter again.", x, y);
        DELAY_SEC(1);
        print_prompt(prompt, x, y);
    }
}

void elvt_ctrl(std::queue<pii>& requests, std::vector<elevator*>& elvts) {
    while(true) {
        screenMutex.lock();
        MVCURSOR(elvts.size()+1, 0);
        CLEAR_UPSCREEN;
        MVCURSOR(0, 0);
        for(int i = 0; i < elvts.size(); i++) {
            elvts[i]->display_floor();
        }
        screenMutex.unlock();
        if(requests.size()) {
            pii req = requests.front();
            int distance = INT32_MAX;
            bool mv_dir = (req.first < req.second); // 1: going up; 0: going down
            elevator* candidate = NULL;
            for(int i = 0; i < elvts.size(); i++) { // choose a elevator that is closest to the customer.
                elevator* elvt = elvts[i];
                int elvt_state = elvt->get_state(), elvt_floot = elvt->get_floor();
                if( (mv_dir && elvt_state == 1 && req.first > elvt_floot && req.first-elvt_floot < distance)  ||  // going up and elvt will arrive current
                    (!mv_dir && elvt_state == 2 && elvt_floot > req.first && elvt_floot-req.first < distance) ||  // going down and elvt will arrive current
                    (elvt_state == 0 && abs(elvt_floot-req.first) < distance ) ) {  // Idle
                    distance = abs(req.first-elvt_floot);
                    candidate = elvt;
                }
            }
            if(candidate) {
                candidate->move(req.first, req.second);
                requests.pop();
            }
        }
        DELAY_MS(100);
    }
}

void rmt_srv(std::vector<elevator*>& elvts) {
    const char* port_str = getenv("SERVER_PORT"); // Get the port from the environment variable, or use the default port
    int port = (port_str != NULL) ? atoi(port_str) : PORT;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Set up the server address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(server_fd);
    }

    if (listen(server_fd, 3) == -1) {
        std::cerr << "Error listening" << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "Server listening on port " << port << std::endl;

    // Accept connections
    int client_socket;
    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);
    char buffer[1024] = {0};
    while (true) {
        if((client_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&client_address_length)) == -1) {
            perror("accept");
            break;
        }
        while (true) {
            recv(client_socket, buffer, 1024, 0);
            int query = atoi(buffer)-1;
            std::string response;
            if (query == -1)
                break;
            if(query >= elvts.size()) 
                response = "Invalid query. Please enter a non-negative integer.";
            else 
                response = "It at floor - " + std::to_string(elvts[query]->get_floor()) + ".";
            send(client_socket, response.c_str(), response.length(), 0);
        }
        close(client_socket);
    }

    close(server_fd);
}


int main(int argc, char* argv[]) {
    // freopen("input.txt", "r", stdin);
    int btm_floor, top_floor, num_elvt;
    do {
        get_int_input(btm_floor, "Enter the bottom floor of building: ", 1, 0);
        get_int_input(top_floor, "Enter the top floor of building: ", 2, 0);
        if(btm_floor > top_floor)
            std::cout << "top floor shouldn't lower than bottom floor.\n";
    } while(btm_floor > top_floor);  // check the top should be higher than bottom
    do {
        get_int_input(num_elvt, "Enter the number of elevator: ", 3, 0);
        if(num_elvt <= 0) 
            std::cout << "The is no elevator? Please enter positive integer.\n";
    } while(num_elvt <= 0);

    printf("The building has %i floors, wherein the top floor is at floor %i and the bottom floor is at floor %i, and has %i elevators.\n",
        top_floor-btm_floor+1, top_floor, btm_floor, num_elvt);

    std::vector<elevator*> elvts(num_elvt);
    for(int i = 0; i < num_elvt; i++)
        elvts[i] = new elevator(i+1, top_floor, btm_floor);
    std::queue<pii> requests;

    std::thread ctrl_center(elvt_ctrl, std::ref(requests), std::ref(elvts));
    std::thread ctrl_srv(rmt_srv, std::ref(elvts));
    MVCURSOR(0,0);
    CLEAR_DOWNSCREEN;
    DELAY_SEC(1);

    int curr, dest;
    while(true) {
        do {
            get_int_input(curr, "Enter your floor: ", num_elvt+1, 0);
            if(curr > top_floor || curr < btm_floor) {
                printf("The floor %i is the top and the floor %i is the bottom.\n", top_floor, btm_floor);
                DELAY_SEC(1);
            }
        } while(curr > top_floor || curr < btm_floor);
        do {
            get_int_input(dest, "Enter the floor you want to go to: ", num_elvt+1, 0);
            if(dest > top_floor || dest < btm_floor) {
                printf("The floor %i is the top and the floor %i is the bottom.\n", top_floor, btm_floor);
                DELAY_SEC(1);
            }
        } while(dest > top_floor || dest < btm_floor);
        if(curr != dest)
            requests.push({curr, dest});
    }
}