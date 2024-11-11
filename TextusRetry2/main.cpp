#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;

const static int COL_MAX = 20; //max ammount of collumns in a terminal window (will be set with config file)
const static int BACKSPACE = 8;	//\b
const static int NEW_LINE = 13;	//\r
const static int COMMAND = 96;	//`																 
const static int ARROW_KEY_SIGNAL = -32; //alpha character, signals that the following value is to be handled as an arrow key
const static int KEY_LEFT = 75;
const static int KEY_RIGHT = 77;
const static int KEY_UP = 72;
const static int KEY_DOWN = 80;

void display(string file, int cursorLocation, string debugString) {
	system("cls");
	int cursorLineEndDelta, i;
	cout << debugString << endl;
	if (cursorLocation > file.length()) {
		cout << file;
		cursorLineEndDelta = cursorLocation - file.length();
		for (i = 0; i < cursorLineEndDelta; i++) cout << " ";
		cout << "|";
	}
	else cout << file.substr(0, cursorLocation) << "|" << file.substr(cursorLocation, file.length());
}

void saveFile(string fileLoader, string path) {
	ofstream fileToSave;
	fileToSave.open(path);
	fileToSave << fileLoader;							  
	fileToSave.close();
}

int main() {
	ifstream mainFile;
	string mainFileString;
	string loaderString;
	string path = "Text.txt";
	mainFile.open(path, ios::app);
	string fileLoader, command, copyString, tempString, debugString;
	char inputChar = 'c';
	int cursorPosition[2] = { 0 };
	int i = 0;
	int randomIterator;
	bool IsCommandCalled = false;
	bool displayCharacterValue = false;
	bool ignoreInput = false;
	int row = 0;
	int distanceFromLineBeginning;
	bool changesAreMade = false;
	int scalarCursorPosition = 0;
	
	while (!mainFile.eof()) {
		getline(mainFile, loaderString);
		mainFileString += loaderString + '\n';
	}

	mainFile.close();
	fileLoader = mainFileString;
	
	for (i = 0; i < fileLoader.length(); i++) {
		if (fileLoader[i] == '\n') {
			cursorPosition[0]++;
			cursorPosition[1] = 0;
		}
		cursorPosition[1]++;
	}
	
	scalarCursorPosition = cursorPosition[0] * COL_MAX + cursorPosition[1];
	
	while (true) {
		if (IsCommandCalled) {
			if (command == "exit") {
				if (changesAreMade) {
					string option;
					cout << "Do you want to save (y\\n): ";
					cin >> option;
					if (option == "y") saveFile(fileLoader, path);
				}
				break;
			}
			else if (command == "help" || command == "?") {
				system("cls");
				cout << "exit - exits the program, starts dialogue for saving" << endl << "help - shows this screen" << endl << "goto <number> - moves cursor to index corresponding to <number>" << endl;
				cout << "CharacterValue - toggles int display for currentChar var" << endl << "save - saves the file" << endl << "clear - clears the file" << endl << "copy <startPos> <endPos> - copies string from startPos and endPos" << endl;
				cout << "paste <pos> - pastes copy string into <pos>" << endl << "value <pos> - writes int value at pos" << endl << endl;
				system("pause");
			}
			else if (command.substr(0, 4) == "goto") {
				scalarCursorPosition = stoi(command.substr(5, command.length() - 5));
				if (cursorPosition[1] < 0) {
					if (cursorPosition[0] != 0) {
						cursorPosition[0]--;
						cursorPosition[1] = COL_MAX;
					}
				}
				command.clear();
			}
			else if (command == "CharacterValue") displayCharacterValue = displayCharacterValue ? displayCharacterValue = false : displayCharacterValue = true;
			else if (command == "save") {
				saveFile(fileLoader, path);
				changesAreMade = false;
			} 
			else if (command == "clear") {
				fileLoader = "";
				cursorPosition[0] = 0;
				cursorPosition[1] = 0;
			}
			else if (command.substr(0, 4) == "copy") {
				int pos1, pos2;
				int j = 5;
				char queryChar = command[j];
				while (queryChar != ' ') {
					tempString += command[j];
					j++;
					queryChar = command[j];
				}
				j++;
				pos1 = stoi(tempString);
				tempString = "";
				pos2 = stoi(command.substr(j, command.length() - j));
				copyString = fileLoader.substr(pos1, (pos2 - pos1));
				cout << copyString << endl;
				system("pause");
			}
			else if (command.substr(0,5) == "paste") {
				if (copyString == "") {
					cout << "You haven't copied over anything yet!";
					system("pause");
					break;
				}
				int pastePosition = stoi(command.substr(6, command.length()-6));
				if (pastePosition + copyString.length() > fileLoader.length()) {
					for (randomIterator = 0; randomIterator < (pastePosition + copyString.length() - fileLoader.length()); randomIterator++) fileLoader += " ";
					fileLoader += copyString;
					cursorPosition += pastePosition + copyString.length() - fileLoader.length();
				}
				else {
					fileLoader = fileLoader.substr(0, pastePosition) + copyString + fileLoader.substr(pastePosition, fileLoader.length());
					cursorPosition += copyString.length();
				}
				
			}
			else if (command.substr(0, 5) == "value") {
				int queryPosition = stoi(command.substr(6, command.length() - 6));
				if (queryPosition > fileLoader.length()) cout << "Out of bounds!";	
				else cout << (int)fileLoader[queryPosition] << endl;
				system("pause");
			}
			else {
				cout << "We don't have that command. Sowwy :(" << endl;
				system("pause");
			}
			IsCommandCalled = false;
		}
		display(fileLoader, cursorPosition, debugString);
		debugString = "";
		if (displayCharacterValue) cout << (int)inputChar << endl;
		inputChar = _getch();
		switch ((int)inputChar) {
		case ARROW_KEY_SIGNAL:
			switch (_getch()) {
			default:
				cout << "SUMN WENT WRONG";
				ignoreInput = true;
				break;
			case KEY_LEFT:
				ignoreInput = true;
				cursorPosition--;
				break;
			case KEY_RIGHT:
				ignoreInput = true;
				cursorPosition++;
				if (cursorPosition > fileLoader.length()) fileLoader += " ";
				break;
			case KEY_UP:
				/*ignoreInput = true;
				distanceFromLineBeginning = cursorPosition - newLinePositionsArray[row];
				row--;
				cursorPosition -= distanceFromLineBeginning;
				distanceFromLineBeginning = cursorPosition - newLinePositionsArray[row];
				for (randomIterator = 0; randomIterator < distanceFromLineBeginning; randomIterator++) {
					tempString += " ";
				}
				fileLoader = fileLoader.substr(0, cursorPosition) + tempString + '\n' + fileLoader.substr(cursorPosition, fileLoader.length() - cursorPosition);
				//debugString = "KEY_UP";*/
				break;
			case KEY_DOWN:
				ignoreInput = true;
				//debugString = "KEY_DOWN";
				break;
			}
		case BACKSPACE:
			changesAreMade = true;
			if (ignoreInput) {
				ignoreInput = false;
				break;
			}
			if (scalarCursorPosition > 0) scalarCursorPosition--;
			else break;
			if (fileLoader[scalarCursorPosition] == '\n') row--;
			fileLoader = fileLoader.substr(0, cursorPosition);
			break;
		case COMMAND:
			IsCommandCalled = true;
			for (randomIterator = 0; randomIterator < 12; randomIterator++) cout << endl;
			cout << "ENTER COMMAND: ";
			getline(cin, command);
			break;
		case NEW_LINE:
			changesAreMade = true;
			fileLoader += '\n';
			scalarCursorPosition += COL_MAX - (scalarCursorPosition % COL_MAX);
			break;
		default:
			changesAreMade = true;
			if (scalarCursorPosition >= fileLoader.length()) fileLoader += inputChar;
			else fileLoader[scalarCursorPosition] = inputChar;
			scalarCursorPosition++;
			break;
		}
		cursorPosition[0] = scalarCursorPosition / COL_MAX;
		cursorPosition[1] = scalarCursorPosition % COL_MAX;
	}
	return 0;	//CONTRIBUTION BY : BaneStano
}