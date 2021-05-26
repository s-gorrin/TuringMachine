/**********************************🐌*🌲*🦕***********************************\
*      file: TuringMachine.cpp                                                *
*      description: implementation for the turing machine class               *
*                                                                             *
*      assignment: implement a turing machine                                 *
*      known bugs: "out_of_range: vector" moving to states outside the table  *
*      term: CIS25, Fall 2020                                                 *
*      name: Seth Gorrin                                                      *
\*****************************************************************************/

#include <iostream>
#include "TuringMachine.h"

const std::string TAPE_ALLOWED = ",1234567890:-";
const std::string STATE_ALLOWED = "01,RL:23456789\n";
const int TAPE_ALLOWED_LEN = 13;
const int STATE_ALLOWED_LEN = 15;
const char ASCII_ZERO = '0';

TuringMachine::TuringMachine() {
	currentSquare = 0;
	currentState = 1;
	tape = {};
	error = '0';
}

TuringMachine::TuringMachine(std::string tapeFile, std::string stateFile) {
	colZero.push_back(stateZero());
	colOne.push_back(stateZero());
	currentState = 1;
	currentSquare = 0;
	error = '0';

	if (softValidate(tapeFile, stateFile)) {
		parseTapeInput(tapeFile);
		parseStateInput(stateFile);
	}
	else {
		std::cout << "There was a problem with an input file." << std::endl;
	}
}

// update machine based on state and current square
void TuringMachine::update() {
	if (currentState && error == '0') {
		if (readSquare()) { // step in if square is marked
			if (!colOne.at(currentState).leaveMark)
				removeMark();
			colOne.at(currentState).moveTo ? moveRight() : moveLeft();
			currentState = colOne.at(currentState).nextState;
		}
		else {
			if (colZero.at(currentState).leaveMark)
				makeMark();
			colZero.at(currentState).moveTo ? moveRight() : moveLeft();
			currentState = colZero.at(currentState).nextState;
		}
	}
}

// update the machine until state zero is reached
void TuringMachine::run() {
	while (currentState && error == '0')
		update();
}

// set up state zero to initialize state table
State TuringMachine::stateZero() {
	State zero;
	zero.leaveMark = false;
	zero.moveTo = false;
	zero.nextState = 0;

	return zero;
}

// check the tape and state files for allowed characters, trim trailing chars
bool TuringMachine::softValidate(std::string &tapeFile, std::string &stateFile) {
	bool flag = false;

	for (int i = 0; i < tapeFile.length(); ++i) {
		for (int j = 0; j < TAPE_ALLOWED_LEN; ++j) {
			if (tapeFile.at(i) == TAPE_ALLOWED.at(j)) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			error = 't';
			return false;
		}
		flag = false;
	}
	while (tapeFile.back() == ',')
		tapeFile.pop_back();

	for (int i = 0; i < stateFile.length(); ++i) {
		for (int j = 0; j < STATE_ALLOWED_LEN; ++j) {
			if (stateFile.at(i) == STATE_ALLOWED.at(j)) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			error = 's';
			return false;
		}
		flag = false;
	}
	while (stateFile.back() == '\n')
		stateFile.pop_back();

	return true;
}

// fill values of currentSquare and tape from input string
void TuringMachine::parseTapeInput(std::string tapeFile) {
	using std::string;

	size_t pos = tapeFile.find(':');
	string trimmedTape;

	currentSquare = std::stoi(tapeFile.substr(0, pos));

	if (pos == string::npos) // for tapes with number and no ':'
		return;

	trimmedTape = tapeFile.substr(++pos, string::npos);

	if (trimmedTape == "") // for tapes with no marked squares
		return;

	while (pos != string::npos) {
		pos = trimmedTape.find(',');
		tape.insert(std::stoi(trimmedTape.substr(0, pos)));
		if (pos != string::npos) // necessary to avoid infinite loop
			trimmedTape = trimmedTape.substr(++pos, string::npos);
	}
}

// fill state tables from input string
void TuringMachine::parseStateInput(std::string stateFile) {
	using std::string;

	if (stateFile.length() < 11)
		currentState = 0; // this is bad, but it's an edge case

	size_t linePos = stateFile.find('\n');

	if (linePos == string::npos) { // for one-line state tables
		string left, right;
		splitCommand(stateFile, left, right);
		parseCommands(left, right);
		return;
	}

	while (linePos != string::npos) {
		linePos = stateFile.find('\n');
		string line = stateFile.substr(0, linePos);
		string left, right;
		splitCommand(line, left, right);
		parseCommands(left, right);

		if (linePos != string::npos) {
			stateFile = stateFile.substr(++linePos, string::npos);
		}
	}
}

// split one state into left and right columns
void TuringMachine::splitCommand(std::string line,
		std::string &left, std::string &right) {
	size_t pos = line.find(':');
	left = line.substr(0, pos);
	right = line.substr(++pos, std::string::npos);
}

// convert data from a line of the state table into struct form
void TuringMachine::parseCommands(std::string left, std::string right) {
	State zero = fillState(left);
	State one = fillState(right);

	colZero.push_back(zero);
	colOne.push_back(one);
}

// fill a state of a single command
State TuringMachine::fillState(std::string command) {
	State state;

	state.leaveMark = command.at(0) - ASCII_ZERO;

	size_t pos = command.find(',');
	command = command.substr(++pos, std::string::npos);

	if (command.at(0) == 'R')
		state.moveTo = true;
	else
		state.moveTo = false;

	pos = command.find(',');
	command = command.substr(++pos, std::string::npos);

	state.nextState = std::stoi(command);

	return state;
}

// returns a string displaying squares with marks on them and current square
std::string TuringMachine::tapeToString() const {
	std::set<int>::iterator it;
	std::string printTape = "Tape: [";
	std::string secondPart = "]\nCurrent square: "
		+ std::to_string(currentSquare);

	if (error == 't')
		return "There was a problem with the tape.";

	for (auto it = tape.begin(); it != tape.end(); it++) {
		printTape += std::to_string(*it);
		printTape.push_back(','); // I did some testing and found that
		printTape.push_back(' '); // two push_backs is faster than append(", ")
	}

	if (printTape.find(',') != std::string::npos) {
		printTape.pop_back();
		printTape.pop_back();
	}
	printTape.append(secondPart);

	return printTape;
}

// returns a string displaying the state table
std::string TuringMachine::statesToString() const {
	std::string stateTable;
	if (error == 's')
		return "There was a problem with the state table.";

	for (int i = 1; i < colZero.size(); i++) {
		std:: string line = "State " + std::to_string(i) + ": ";
		stateTable += line;
		stateTable += (colZero.at(i).leaveMark ? "1," : "0,");
		stateTable += (colZero.at(i).moveTo ? "R," : "L,");
		stateTable.append(std::to_string(colZero.at(i).nextState));
		stateTable.push_back(':'); // between sides of the table here
		stateTable += (colOne.at(i).leaveMark ? "1," : "0,");
		stateTable += (colOne.at(i).moveTo ? "R," : "L,");
		stateTable.append(std::to_string(colOne.at(i).nextState));
		stateTable.push_back('\n');
	}
	stateTable.pop_back();

	return stateTable;
}
