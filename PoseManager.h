#ifndef POSMANAGER_H
#define POSMANAGER_H

#include "Joint.h"

/* PoseManager class contains five Joint objects running on different threads
*  so all five joints can be manipulated through one PoseManager object
*/
class PoseManager {

private:
  Joint *base;
  Joint *bicep;
  Joint *elbow;
  Joint *wrist;
  Joint *gripper;

public:
  // Class constructor
  // Initializes the five joints at a straight up pose
  PoseManager();

  // Class destructor
  ~PoseManager();

  void JustBase(int deg, int dur);
  void JustBicep(int deg, int dur);
  void JustElbow(int deg, int dur);
  void JustWrist(int deg, int dur);
  void JustGripper(int deg, int dur);

  // Creates a pose with five instances of the joint class
  void MakePose(int *degs, int *durs);
};

#endif
