#include "PoseManager.h"
#include "Joint.h"
#include <iostream>

using namespace std;

PoseManager::PoseManager() {
  // Initializes five joints to a straight up position

  cout << "Pre init" << endl;
  base = new Joint(13);
  bicep = new Joint(10);
  elbow = new Joint(11);
  wrist = new Joint(12);
  gripper = new Joint(0);

cout << "Pre moveto" << endl;

	base->MoveTo(90, 0);
	bicep->MoveTo(90, 0);
	elbow->MoveTo(90, 0);
	wrist->MoveTo(90, 0);
	gripper->MoveTo(0, 0);

cout << "Pre start" << endl;
  // Starts PWM signal to each of the joints
  base->Start();
  bicep->Start();
  elbow->Start();
  wrist->Start();
  gripper->Start();
}

void PoseManager::JustBase(int deg, int dur) { base->MoveTo(deg, dur);}

void PoseManager::JustBicep(int deg, int dur) { bicep->MoveTo(deg, dur);}

void PoseManager::JustElbow(int deg, int dur) { elbow->MoveTo(deg, dur);}

void PoseManager::JustWrist(int deg, int dur) { wrist->MoveTo(deg, dur);}

void PoseManager::JustGripper(int deg, int dur) { gripper->MoveTo(deg, dur);}

void PoseManager::MakePose(int *degs, int *durs) {

  JustBase(degs[0], durs[0]);
  JustBicep(degs[1], durs[1]);
  JustElbow(degs[2], durs[2]);
  JustWrist(degs[3], durs[3]);
  JustGripper(degs[4], durs[4]);
}

PoseManager::~PoseManager() {

  delete base;
  delete bicep;
  delete elbow;
  delete wrist;
  delete gripper;
}
