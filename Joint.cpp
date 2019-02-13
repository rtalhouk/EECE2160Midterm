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

Joint::Joint(int number) {
  // Joint device files will follow the format
  //	/sys/class/gpio/gpio<NR>/value
  // <NR> has to be replaced with the actual device number passed as an
  // argument to the class constructor.
  char device_name[128];
  sprintf(device_name, "/sys/class/gpio/gpio%d/value", number);

  // Open special device file and store file descriptor in class member.
  fd = open(device_name, O_WRONLY);
  if (fd < 0) {
    std::cerr << "Cannot open " << device_name << " - forgot sudo?\n";
    exit(1);
  }
  std::cout << "Opened " << number << std::endl;
  kill_thread = true;
  period = 20000;
}

Joint::~Joint() {
  // Close open file descriptor
  Stop();
  close(fd);
}

void Joint::Start() {
  kill_thread = false;
  pwm_thread = std::thread(PWMThread, fd, std::ref(period), std::ref(target_pulse),
                       std::ref(cur_pulse), std::ref(transition_periods),
                       std::ref(kill_thread));
}

void Joint::Stop() {
  kill_thread = true;
  pwm_thread.join();
}

void Joint::MoveTo(int pos, int duration) {
  std::cout << "Moving to " << pos << " over duration " << duration << std::endl;
  target_pulse = deg_to_pulse(period, pos);
  transition_periods = duration*1000 / period;
}

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

int deg_to_pulse(int period, int angle) { return angle * 1800 / 180 + 600; }
