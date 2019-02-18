#include "PoseManager.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace std;

//ZedBoard specific constants needed to interface with the hardware
const unsigned gpio_address = 0x400d0000;
const unsigned gpio_size = 0xff;
const int gpio_pbcenter = 0x17C;

//Professor-supplied initialization function to establish hardware communication
char *Initialize(int *fd) {
  *fd = open("/dev/mem", O_RDWR);
  return (char *) mmap(
	NULL,
	gpio_size,
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	*fd,
	gpio_address);
}

//Reads values from the ZedBoard sensors
int RegisterRead(char *pBase, int offset) {
  return * (int *) (pBase + offset);
}

/* Main function
*  Params: argc, argv: command line arguments, count and values
*  Directs the robotic arm to pick up and throw a plastic bottle
*/
int main(int argc, char const *argv[]) {
  int fd;
  char *pBase = Initialize(&fd);
  if (pBase == MAP_FAILED) {
    perror("Mapping I/O memory failed - Did you run w/sudo?\n");
    return -1;
  }

  cout << "Initializing pose manager" << endl;
  PoseManager pm;
  //Angles found to yeild a straight-up position of the arm
  int angles[5] = {85, 76, 100, 90, 0};
  int durations[5] = {0,0,0,0,0};
  pm.MakePose(angles, durations);

  //delays the start of the arm movement until a button on the ZedBoard is pressed
  bool started = false;
  while(!started) {
	if(RegisterRead(pBase, gpio_pbcenter)) started = true;
	usleep(10000);
  }

  //for loop directed the arm to move to different angles specified by a text
  //file fed into the program. Technique for reading lines found on StackOverflow
  for (string s; getline(cin, s); ) {
	std::stringstream is( s );

	int input[7] = {0};
	int i = 0, n;
	while(is >> n) {
		input[i] = n;
		i ++;
	}
	if(i != 7) {
		cout << "Skipping input" << endl;
		continue;
	}

	int angles[5] = {input[0], input[1], input[2], input[3], input[4]};
	int durations[5] = {input[5], input[5], input[5], input[5], input[5]};

	pm.MakePose(angles, durations);

	std::this_thread::sleep_for(std::chrono::milliseconds(input[6]));
	cout << "Finished input" << endl;
  }

  return 0;
}
