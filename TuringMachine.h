/**********************************🐌*🌲*🦕***********************************\
*      file: TuringMachine.h                                                  *
*      description: definition of a turing machine class                      *
*                                                                             *
*      assignment: implement a turing machine                                 *
*      term: CIS25, Fall 2020                                                 *
*      name: Seth Gorrin                                                      *
\*****************************************************************************/

#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <string>
#include <vector>
#include <set>

struct State {
	bool leaveMark; // true for mark, false for no mark
	bool moveTo;    // True for righT, False for leFt
	unsigned int nextState;
};

class TuringMachine {
	private:
		std::set<long long> tape;
		long long currentSquare;
		unsigned int currentState;
		std::vector<State> colZero;
		std::vector<State> colOne;
		char error; // s for state file, t for tape file, 0 for fine

		State stateZero();
		bool softValidate(std::string &tapeFile, std::string &stateFile);

		void parseTapeInput(std::string tapeFile);

		void parseStateInput(std::string stateFile);
		void splitCommand(std::string line,
				std::string &left, std::string &right);
		void parseCommands(std::string zero, std::string one);
		State fillState(std::string command);

	public:
		TuringMachine();
		TuringMachine(std::string tapeFile, std::string stateFile);

		void update();
		void run();

		// mutators
		void moveLeft() {
			currentSquare--;
		}

		void moveRight() {
			currentSquare++;
		}

		void makeMark() {
			tape.insert(currentSquare);
		}

		void removeMark() {
			tape.erase(currentSquare);
		}

		// accessors
		bool readSquare() const { // true for mark, false for no mark
			return tape.find(currentSquare) != tape.end();
		}

		unsigned int getState() const {
			return currentState;
		}

		std::string tapeToString() const;
		std::string statesToString() const;
};



#endif // TURING_MACHINE_H
