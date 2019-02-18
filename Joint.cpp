#include "Joint.h"
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <utility>


void PWMThread(int fd, int &period, int &target_pulse, int &cur_pulse,
               int &transition_periods, bool &kill_thread);
int deg_to_pulse(int period, int angle);


/* Class constructor
*  Param: number either 0, 10, 11, 12, or 13 represents the port number
*  for each servo connected to the ZedBoard
*/
Joint::Joint(int number) {
  char device_name[128];
  sprintf(device_name, "/sys/class/gpio/gpio%d/value", number);

  // Open special device file and store file descriptor in class field fd
  fd = open(device_name, O_WRONLY);
  if (fd < 0) {
    std::cerr << "Cannot open " << device_name << " - forgot sudo?\n";
    exit(1);
  }
  std::cout << "Opened " << number << std::endl;
  kill_thread = true;
  period = 20000;
}

//Class destructor
Joint::~Joint() {
  // Close open file descriptor
  Stop();
  close(fd);
}

/* Starts the PWM signal to the joint specified by the file descriptor field
*  of this joint
*/
void Joint::Start() {
  kill_thread = false;
  pwm_thread = std::thread(PWMThread, fd, std::ref(period), std::ref(target_pulse),
                       std::ref(cur_pulse), std::ref(transition_periods),
                       std::ref(kill_thread));
}


/* Starts the PWM signal to the joint specified by the file descriptor field
*  of this joint and joins this thread with other terminating threads
*/
void Joint::Stop() {
  kill_thread = true;
  pwm_thread.join();
}

/* Mutates the target_pulse and transition_periods fields of this joint to signal
*  the speed at which the servo should move to a new position
*/
void Joint::MoveTo(int pos, int duration) {
  std::cout << "Moving to " << pos << " over duration " << duration << std::endl;
  target_pulse = deg_to_pulse(period, pos);
  transition_periods = duration*1000 / period;
}

/* Manages the on and off time of the PWM signal and changes the ratios
* depending on the target pulse and duration
*/
void PWMThread(int fd, int &period, int &target_pulse, int &cur_pulse,
               int &transition_periods, bool &kill_thread) {
  while (!kill_thread) {

    if (transition_periods == 0)
      cur_pulse = target_pulse;
    else {
      cur_pulse += (target_pulse - cur_pulse) / transition_periods;
      transition_periods--;
    }

    write(fd, "1", 1);
    usleep(cur_pulse);
    write(fd, "0", 1);
    usleep(period - cur_pulse);
  }

  write(fd, "0", 1);
}

//Converts angle of servo extension (in degrees) to PWM pulse width in milliseconds
int deg_to_pulse(int period, int angle) { return angle * 1800 / 180 + 600; }
