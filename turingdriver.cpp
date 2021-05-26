/**********************************🐌*🌲*🦕***********************************\
*      file: turingdriver.cpp                                                 *
*      description: test main for the turing machine class                    *
*                                                                             *
*      assignment: implement a turing machine                                 *
*      term: CIS25, Fall 2020                                                 *
*      name: Seth Gorrin                                                      *
\*****************************************************************************/

#include <iostream>
#include <fstream>
#include <time.h>
#include "TuringMachine.h"

using std::cout;
using std::endl;

const std::string TAPE_FILE = "test_files/tape_file.txt";
//const std::string STATE_FILE = "test_files/state_file.txt";
const std::string STATE_FILE = "test_files/no_state.txt";
//const std::string STATE_FILE = "test_files/numdup.txt";
const int BIG_NUM = 1000000;

void functionalityTests(TuringMachine &alan) {
	for (int i = 0; i < 3; i++) {
		alan.moveLeft();
	}

	for (int i = 0; i < 6; i++) {
		alan.makeMark();
		alan.moveRight();
	}
	
	cout << alan.tapeToString() << endl;

	alan.moveLeft();
	while (alan.readSquare()) {
		alan.moveLeft();
	}
	alan.moveRight();
	alan.makeMark(); // no problems making an existing mark

	alan.removeMark();
	alan.removeMark(); // no problems removing an non-existent mark

	cout << endl << "current (0): " << alan.readSquare() << endl;

	alan.moveRight();

	cout << "current (1): " << alan.readSquare() << endl << endl;
	cout << alan.tapeToString() << endl;
}

void stressTest(TuringMachine &alan) {
	for (int i = 0; i < BIG_NUM; i++) {
		alan.moveRight();
		alan.makeMark();
	}
	
	for (int i = 0; i < BIG_NUM; i++) {
		alan.moveLeft();
		alan.removeMark();
	}
}

int main() {
//	clock_t before = clock();
	TuringMachine alan;
	std::ifstream tapeFile(TAPE_FILE);
	std::ifstream stateFile(STATE_FILE);
	std::string tapeIn;
	std::string stateIn;
	
	alan.makeMark();
	for (int i = 0; i < 2147483647; i++) {
		alan.moveRight();
	}
	alan.moveRight();
	alan.makeMark();
	cout << alan.tapeToString() << endl;
/*
	std::string line;
	getline(tapeFile, line);
	tapeIn.append(line);

	if (stateFile) {
		while (!stateFile.eof()) {
			getline(stateFile, line);
			stateIn.append(line);
			stateIn.push_back('\n');
		}
		stateIn.pop_back();
		stateIn.pop_back();
	}

	TuringMachine ada(tapeIn, stateIn);
//	functionalityTests(alan);
	cout << ada.statesToString() << endl;

	clock_t timer = clock();
	std::string prevStr = "";
	while (ada.getState()) {
		std::string tapeStr = ada.tapeToString();
		size_t pos = tapeStr.find('\n');
		tapeStr = tapeStr.substr(0, pos);
		if (tapeStr != prevStr) {
			cout << ada.tapeToString() << endl;
			prevStr = tapeStr;
		}
		ada.update();
		if (clock() > timer + (CLOCKS_PER_SEC * 30))
			break;
	}

	cout << ada.tapeToString() << endl;

	tapeFile.close();
	stateFile.close();
*/
	/*
	clock_t after = clock();
	clock_t subtractStart = clock();
	cout << endl << "Time taken: " << after - before <<
		" ticks, at " << CLOCKS_PER_SEC << " ticks per second." << endl;
	clock_t subtractEnd = clock();

	cout << "[stress test: adding and removing "
		<< BIG_NUM << " marks in two loops]" << endl;

	stressTest(alan);

	after = clock();
	int sub = subtractEnd - subtractStart;

	cout << "Total time taken: " << (after - before) - sub <<
		" ticks, at " << CLOCKS_PER_SEC << " ticks per second." << endl;
	*/

	return 0;
}
