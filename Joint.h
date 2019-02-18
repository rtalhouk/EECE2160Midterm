#ifndef JOINT_H
#define JOINT_H
#include <thread>

//Represents the state of one of the five servo joints on a robotic arm
class Joint {
  // File descriptor
  int fd;
  //Proccesor thread
  std::thread pwm_thread;
  //Should the thread stop running?
  bool kill_thread;
  //Period of the PWM signal, constant at 20000us
  int period;
  //On-time of the desired PWM signal
  int target_pulse;
  //On-time of the current PWM signal
  int cur_pulse;
  //Number of periods to take between current and desired PWM on-time
  int transition_periods;

public:
  /**
   * Class constructor.
   *
   * @param number
   *	Port number for GPIO.
   */
  Joint(int number);

  /**
   * Class destructor.
   */
  ~Joint();

  /*
  * Starts the PWM signal running to this thread and servo joint
  */
  void Start();

  /*
  * Stops the PWM signal running to this thread and servo joint
  */
  void Stop();

  /**
   * Sets this joint to move to the given position over the given duration.
   *
   * @param pos the position in degrees to move to
   * @param duration the duration in milliseconds over which to move to the new
   * position
   */
  void MoveTo(int pos, int duration);
};

#endif
