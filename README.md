# Prepare
1. install g++
  ```
  sudo apt install build-essential
  ```
2. compile
  ```
  cd elevator
  make
  ```
  you will get two applications: elevator_app and monitor_app.

# Execute elevator
  This is main execution for simulating the elevator's behavior.
  ```
  cd elevator
  ./elvator_app
  ```
  The elevator_app will establish a query server for remote query of the elevator status.
  If specifying server port is needed (initial port is 8080), please export variable before you start elevator_app, such as
  ```
  export SERVER_PORT <port number>
  ```

  After elevator_app successfully started, it will ask user the top floor, bottom floor and the number of elevator, these questions are initial necessary information and it just ask one time.
  And then, it will continus to ask user new requirment, and continus to update the state of each elevator.

# Execute monitor
  This is used to let use can quiry the state of elevators remotely.
  ```
  cd elevator
  ./monitor_app
  ```
  The monitor_app will connect to query server.
  If specifying server ip and port is needed (initial ip is 127.0.0.1 and port is 8080), please export variable before you start elevator_app, such as
  ```
  export SERVER_HOST <IP>
  export SERVER_PORT <port number>
  ```
