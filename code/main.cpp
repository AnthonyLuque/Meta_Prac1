#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

int main() {
	string line;
	ifstream myfile("ejemplo.txt");
	if (myfile.is_open())
	{

		// Getting the matrix size
		getline(myfile, line);
		int matrixSize = std::stoi(line);

		// Declaring the matrixes
		int** flowMatrix = new int*[matrixSize]; //col
		for (int i = 0; i < matrixSize; ++i)
			flowMatrix[i] = new int[matrixSize]; //row

		int** distanceMatrix = new int*[matrixSize]; //col
		for (int i = 0; i < matrixSize; ++i)
			distanceMatrix[i] = new int[matrixSize]; //row

		// Filling the matrixes in
		int lineNumber = 2;
		while (getline(myfile, line))
		{
			// Flow matrix
			if (3 <= lineNumber && lineNumber < 3 + matrixSize) {
				int i = 0;
				do {
					std::vector<std::string> v;
					split(line, v, ' ');
					for (int j = 0; j < matrixSize; j++) {
						flowMatrix[i][j] = std::stoi(v[j]);
					}
					// New line
					getline(myfile, line);
					lineNumber++;
					i++;
				} while (i < matrixSize);
			}

			// Distance matrix
			if (4 + matrixSize <= lineNumber && lineNumber < 4 + matrixSize + matrixSize) {
				int i = 0;
				do {
					std::vector<std::string> v;
					split(line, v, ' ');
					for (int j = 0; j < matrixSize; j++) {
						distanceMatrix[i][j] = std::stoi(v[j]);
					}
					// New line
					getline(myfile, line);
					lineNumber++;
					i++;
				} while (i < matrixSize);
			}
			lineNumber++;
		}
		myfile.close();

		// Displaying the matrixes
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				cout << " " << flowMatrix[i][j];
			}
			cout << endl;
		}

		cout << endl;
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				cout << " " << distanceMatrix[i][j];
			}
			cout << endl;
		}

	}
	else {
		cout << "Unable to open file";
	}

	system("pause");
	return 0;
}
