class elevator {
public:
    elevator(int number, int floor);
    void display_floor();    // a method that displays the current floor of the elevator
    void move(int current, int floor);  // a method that moves the elevator to the specified floor
    void move2floor(int floor); // a method that moves the elevator to the specified floor
    int get_current_floor();
private:
    int current_floor;  // an integer representing the current floor of the elevator
    int number;
};