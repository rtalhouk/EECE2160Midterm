#include "PoseManager.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
  cout << "Initializing pose manager" << endl;

  PoseManager pm;

  cout << "Initializing pose manager2" << endl;
  while (true) {
    cout << "Select a joint:" << endl
         << "1. Base" << endl
         << "2. Bicep" << endl
         << "3. Elbow" << endl
         << "4. Wrist" << endl
         << "5. Gripper" << endl;

    int joint;
    cin >> joint;

    cout << "Angle:" << endl;

    int angle;
    cin >> angle;

    switch (joint) {
    case 1:
      pm.JustBase(angle, 0);
	break;
    case 2:
      pm.JustBicep(angle, 0);
	break;
    case 3:
      pm.JustElbow(angle, 0);
	break;
    case 4:
      pm.JustWrist(angle, 0);
	break;
    case 5:
      pm.JustGripper(angle, 0);
	break;
    default:
      cout<<"Please Enter a valid number" << endl;
    }
  }

  return 0;
}
