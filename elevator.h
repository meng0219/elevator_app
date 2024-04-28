#include <queue>
#define DELAY_SEC(x) std::this_thread::sleep_for(std::chrono::seconds(x))
#define DELAY_MS(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))
#define CLEAR_UPSCREEN std::cout << "\033[1J"  // Clear the rows before the current cursor
#define CLEAR_DOWNSCREEN std::cout << "\033[J"  // Clear the rows after the current cursor
#define MVCURSOR(x, y) std::cout << "\033["<<x<<";"<<y<<"H" // Move the cursor to the specific position (x, y)

class elevator {
public:
    elevator(int number, int top, int btm);
    void run();
    void display_floor();    // a method that displays the current floor of the elevator
    void move(int current, int floor);  // a method that moves the elevator to the specified floor
    int get_elvt_no();  // to get the elevator's number
    int get_floor();    // to get the current floor that the elevator is on
    int get_state();    // to get the state of the elevator. (0: idle, 1: going up, 2: going down)
private:
    int current_floor;  // an integer representing the current floor of the elevator
    int elvt_no;    // the elevator number
    int top_floor;  // the top floor that the elevator can reach
    int btm_floor;  // the bottom floor that the elevator can reach
    int state;      // the state of the elevator. (0: idle, 1: going up, 2: going down)
    std::priority_queue<int> down_floors;
    std::priority_queue<int, std::vector<int>, std::greater<int>> up_floors;
};