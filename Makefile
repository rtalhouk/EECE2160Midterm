posesequence: PoseMain.o PoseManager.o Joint.o
	g++ -std=c++0x -pthread -o posesequence PoseMain.o PoseManager.o Joint.o

PoseMain.o: PoseMain.cpp PoseManager.h
	g++ -std=c++0x -c PoseMain.cpp

posetester: PoseTester.o PoseManager.o Joint.o
	g++ -std=c++0x -pthread -o posetester PoseTester.o PoseManager.o Joint.o

PoseTester.o: PoseTester.cpp PoseManager.h
	g++ -std=c++0x -c PoseTester.cpp

PoseManager.o: PoseManager.h PoseManager.cpp Joint.h
	g++ -std=c++0x -c PoseManager.cpp

Joint.o: Joint.h Joint.cpp
	g++ -std=c++0x -c Joint.cpp

clean:
	rm *.o

