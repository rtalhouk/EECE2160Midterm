#ifndef JOINT_H
#define JOINT_H
#include <thread>

class Joint {
  // File descriptor
  int fd;
  std::thread pwm_thread;
  bool kill_thread;
  int period;
  int target_pulse;
  int cur_pulse;
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
  
  void Start();
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
