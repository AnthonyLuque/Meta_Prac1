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



	/* SETTING AND FILLING THE MATRICES IN */


	string line;
	ifstream myfile("datos/cnf01.dat");
	if (myfile.is_open())
	{

		// Getting the matrix size
		getline(myfile, line);
		int matrixSize = std::stoi(line);

		// Declaring the matrixes
		int** flowMatrix = new int*[matrixSize]; //col
		for (int i = 0; i < matrixSize; i++)
			flowMatrix[i] = new int[matrixSize]; //row

		int** distanceMatrix = new int*[matrixSize]; //col
		for (int i = 0; i < matrixSize; i++)
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
					v.clear();
					split(line, v, ' ');

					// Clearing blank data (due to double spaces)
					std::vector<std::string> valuesFlow;
					valuesFlow.clear();
					for (int j = 0; j < v.size(); j++) {
						if (!(v[j] == "")) {
							valuesFlow.push_back(v[j]);
						}
					}
					for (int j = 0; j < matrixSize; j++) {
						flowMatrix[i][j] = std::stoi(valuesFlow[j]);
					}

					// New line
					getline(myfile, line);
					lineNumber++;
					i++;
					cout << endl;
				} while (i < matrixSize);
			}

			// Distance matrix
			if (4 + matrixSize <= lineNumber && lineNumber < 4 + matrixSize + matrixSize) {
				int i = 0;
				do {
					std::vector<std::string> v;
					v.clear();
					split(line, v, ' ');

					// Clearing blank data (due to double spaces)
					std::vector<std::string> valuesDistances;
					valuesDistances.clear();
					for (int j = 0; j < v.size(); j++) {
						if (!(v[j] == "")) {
							valuesDistances.push_back(v[j]);
						}
					}
					for (int j = 0; j < matrixSize; j++) {
						distanceMatrix[i][j] = std::stoi(valuesDistances[j]);
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


		// Displaying the matrices
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





		/* GREEDY ALGORITHM */

		// Variables declaration
		int* flowSums = new int[matrixSize];
		int* distanceSums = new int[matrixSize];

		bool* flowMask = new bool[matrixSize];
		bool* distanceMask = new bool[matrixSize];

		int* greedyUnitAndLocationAssociation = new int[matrixSize];

		// Initialization
		for (int i = 0; i < matrixSize; i++) {
			greedyUnitAndLocationAssociation[i] = 0;
			flowSums[i] = 0;
			distanceSums[i] = 0;
			flowMask[i] = 0;
			distanceMask[i] = 0;
		}

		// Sums calcul
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {

				flowSums[i] += flowMatrix[i][j];
				distanceSums[i] += distanceMatrix[i][j];

			}
		}

		int biggestFreeUnitSum;
		int smallestFreeLocationSum;
		int biggestFreeUnitPosition;
		int smallestFreeLocationPosition;

		// Unit and Location association
		int iterations = 0;
		do {	

			// Initialization
			biggestFreeUnitSum = -1;
			smallestFreeLocationSum = -1;
			biggestFreeUnitPosition = -1;
			smallestFreeLocationPosition = -1;

			// Biggest unit and smallest location sum search
			for (int i = 0; i < matrixSize; i++) {

				if (flowMask[i] == 0) {
					if (biggestFreeUnitSum == -1) {
						biggestFreeUnitSum = flowSums[i];
						biggestFreeUnitPosition = i;
					}
					else if (flowSums[i] > biggestFreeUnitSum) {
						biggestFreeUnitSum = flowSums[i];
						biggestFreeUnitPosition = i;
					}
				}

				if (distanceMask[i] == 0) {
					if (smallestFreeLocationSum == -1) {
						smallestFreeLocationSum = distanceSums[i];
						smallestFreeLocationPosition = i;
					}
					else if (distanceSums[i] < smallestFreeLocationSum) {
						smallestFreeLocationSum = distanceSums[i];
						smallestFreeLocationPosition = i;
					}
				}

			}

			greedyUnitAndLocationAssociation[biggestFreeUnitPosition] = smallestFreeLocationPosition; // Real position
			flowMask[biggestFreeUnitPosition] = 1;
			distanceMask[smallestFreeLocationPosition] = 1;

			iterations++;
		} while (iterations < matrixSize);

		// Displaying the associations
		cout << endl;
		cout << "Greedy Unit And Location Association: " << endl;
		for (int i = 0; i < matrixSize; i++) {
			cout << " " << greedyUnitAndLocationAssociation[i] + 1;
		}
		cout << endl;

		float greedyCost = 0;
		for (int i = 0; i < matrixSize; i++) {
			for (int j = 0; j < matrixSize; j++) {
				if (j != i) {
					greedyCost += flowMatrix[i][j] * distanceMatrix[greedyUnitAndLocationAssociation[i]][greedyUnitAndLocationAssociation[j]];
				}
			}
		}
		cout << "Greedy Cost : " << greedyCost << endl;



		/* BEST FIRST ALGORITHM */

		int* bestFirstUnitAndLocationAssociation = new int[matrixSize];


	}
	else {
		cout << "Unable to open file";
	}


	system("pause");
	return 0;
}
