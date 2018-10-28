/*
 * Instance.cpp
 *
 *  Created on: Oct 1, 2018
 *      Author: melthalas
 */

#include "Instance.h"

Instance::Instance(string path) {
	string line;
		ifstream myfile(path);
		matrixSize=0;
		if (myfile.is_open())
		{

			// Getting the matrix size
			getline(myfile, line);
			matrixSize = std::stoi(line);

			// Declaring the matrixes
			flowMatrix = new int*[matrixSize]; //col
			for (int i = 0; i < matrixSize; ++i)
				flowMatrix[i] = new int[matrixSize]; //row

			distanceMatrix = new int*[matrixSize]; //col
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
							if(valuesFlow.size()!=0)
								flowMatrix[i][j] = std::stoi(valuesFlow[j]);
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
							if(valuesDistances.size()!=0)
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

		}
		else{
			cout << "Unable to open file";
		}
}


void Instance::display(){
	if (matrixSize!=0){
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
}
Instance::~Instance() {
	// TODO Auto-generated destructor stub
}

int* Instance::greedy(int * cost){
	// Variables declaration
	int* flowSums = new int[matrixSize];
	int* distanceSums = new int[matrixSize];
	bool* flowMask = new bool[matrixSize];
	bool* distanceMask = new bool[matrixSize];

	int* unitAndLocationAssociation = new int[matrixSize];
	// Initialization
	for (int i = 0; i < matrixSize; i++) {
		unitAndLocationAssociation[i] = 0;
		flowSums[i] = 0;
		distanceSums[i] = 0;
		flowMask[i] = 0;
		distanceMask[i] = 0;
	}

	// Sums calculation
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

		unitAndLocationAssociation[biggestFreeUnitPosition] = smallestFreeLocationPosition + 1; // Real position
		flowMask[biggestFreeUnitPosition] = 1;
		distanceMask[smallestFreeLocationPosition] = 1;

		iterations++;

		} while (iterations < matrixSize);

		//Calculation of the cost of the generated solution
		for(int i=0;i<matrixSize;i++){
			for(int j=0;j<matrixSize;j++){
				if(i!=j)
					*cost+=flowMatrix[i][j] * distanceMatrix[unitAndLocationAssociation[i]-1][unitAndLocationAssociation[j]-1];
			}
		}

		// Displaying the associations
			cout << endl;
			for (int i = 0; i < matrixSize; i++) {
				cout << unitAndLocationAssociation[i] << " ";
			}
			cout << "coste: " << *cost << endl;
			return unitAndLocationAssociation;
}
int * Instance::bestFirst(int * cost , ofstream &outfile){
	int costDiff,swap;
	*cost=0;
	int * DLB= new int[matrixSize];
	bool improve_flag;
	for(int i=0;i<matrixSize;i++){
		DLB[i]=0;
	}

	int* unitAndLocationAssociation = new int[matrixSize];

	/*First random solution*/
	for(int i=0; i<matrixSize; i++){

		unitAndLocationAssociation[i]= i+1;
	}
	random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);
	//Calculation of the cost of the generated solution
	for(int i=0;i<matrixSize;i++){
		for(int j=0;j<matrixSize;j++){
			if(i!=j)
				*cost+=flowMatrix[i][j] * distanceMatrix[unitAndLocationAssociation[i]-1][unitAndLocationAssociation[j]-1];
		}
	}
	outfile << "Initial solution cost: " << *cost << endl;
	//Main loop
	int it = 0;
	while(it<50000 && checkDLB(DLB)){
		for(int i=0; i<matrixSize;i++){
			if(DLB[i]==0){
				improve_flag=false;
				for (int j=0;j<matrixSize; j++){
					if(j!=i && DLB[j]==0){
						costDiff = checkMove(unitAndLocationAssociation,i,j);
						it++;
						if(costDiff<0){
							//Making the change effective
							swap = unitAndLocationAssociation[i];
							unitAndLocationAssociation[i] = unitAndLocationAssociation[j];
							unitAndLocationAssociation[j] = swap;
							*cost+=costDiff;

							DLB[i]=0;
							DLB[j]=0;
							improve_flag=true;

							outfile << "Movement in iteration: " << it << ", new cost: " << *cost << endl;
						}
					}
				}
				if (!improve_flag)
					DLB[i]=1;
			}
		}
	}
	if(it>50000)
		outfile << "Loop ended because of iteration number" <<endl;
	else
		outfile << "Loop ended because of DLB"<< endl;

	//Display solution and cost
	for(int i=0; i< matrixSize; i++){
		cout << unitAndLocationAssociation[i] << " ";
	}
	cout << "coste: "<< *cost << endl;

	return unitAndLocationAssociation;
}

bool Instance::checkDLB(int * DLB){
	for(int i=0;i<matrixSize;i++){
		if(DLB[i]==0)
			return true;
	}
	return false;
}

int Instance::checkMove(int * sol, int i, int j){
	int cost=0;
	for(int k=0;k<matrixSize;k++){
		if(k!=i && k!=j){
			cost+=flowMatrix[i][k] * (distanceMatrix[sol[j]-1][sol[k]-1] - distanceMatrix[sol[i]-1][sol[k]-1]);
			cost+=flowMatrix[j][k] * (distanceMatrix[sol[i]-1][sol[k]-1] - distanceMatrix[sol[j]-1][sol[k]-1]);
			cost+=flowMatrix[k][i] * (distanceMatrix[sol[k]-1][sol[j]-1] - distanceMatrix[sol[k]-1][sol[i]-1]);
			cost+=flowMatrix[k][j] * (distanceMatrix[sol[k]-1][sol[i]-1] - distanceMatrix[sol[k]-1][sol[j]-1]);
		}
	}
	return cost;
}

int * Instance::simAnnealingBoltzmann(int * cost , ofstream &outfile){
	int costDiff,swap;
	*cost=0;
	int * DLB= new int[matrixSize];
	int * bestSolution= new int[matrixSize];
	int bestCost = 0;
	bool improve_flag;
	for(int i=0;i<matrixSize;i++){
		DLB[i]=0;
	}

	int* unitAndLocationAssociation = new int[matrixSize];

	/*First random solution*/
	for(int i=0; i<matrixSize; i++){

		unitAndLocationAssociation[i]= i+1;
	}
	random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);
	//Calculation of the cost of the generated solution
	for(int i=0;i<matrixSize;i++){
		for(int j=0;j<matrixSize;j++){
			if(i!=j)
				*cost+=flowMatrix[i][j] * distanceMatrix[unitAndLocationAssociation[i]-1][unitAndLocationAssociation[j]-1];
		}
	}
	double initialT= (double)*cost * 1.5;
	double T=initialT;

	outfile << "Initial solution cost: " << *cost << ", Initial temperature: "<<initialT << endl;
	//Main loop
	int it = 0;
	while(it<50000 && checkDLB(DLB) && T>(5.0*initialT)/100.0){
		for(int i=0; i<matrixSize;i++){
			if(DLB[i]==0){
				improve_flag=false;
				for (int j=0;j<matrixSize; j++){
					if(j!=i && DLB[j]==0){
						costDiff = checkMove(unitAndLocationAssociation,i,j);
						it++;

						if(costDiff<0){
							//Making the change effective
							swap = unitAndLocationAssociation[i];
							unitAndLocationAssociation[i] = unitAndLocationAssociation[j];
							unitAndLocationAssociation[j] = swap;
							*cost+=costDiff;

							bestSolution = unitAndLocationAssociation;
							bestCost=*cost;
							DLB[i]=0;
							DLB[j]=0;
							improve_flag=true;
							outfile << "Movement in iteration: " << it << ", new cost: " << *cost << endl;
						}else{
							/*Simulated annealing*/
							double pAcceptance = exp(((double)costDiff)/T);
							if(T>pAcceptance){
								//Making the change effective
								swap = unitAndLocationAssociation[i];
								unitAndLocationAssociation[i] = unitAndLocationAssociation[j];
								unitAndLocationAssociation[j] = swap;
								*cost+=costDiff;
								DLB[i]=0;
								DLB[j]=0;
								outfile << "Worse movement in iteration: " << it << ", new cost: " << *cost << endl;
							}
						}

					}
					T= T / (1+log(it));
				}
				if (!improve_flag)
					DLB[i]=1;
			}

		}

	}
	if(it>50000)
		outfile << "Loop ended because of iteration number" <<endl;
	else if(!checkDLB(DLB))
		outfile << "Loop ended because of DLB"<< endl;
	else
		outfile << "Loop ended because of the temperature"<< endl;
	//Display solution and cost
	for(int i=0; i< matrixSize; i++){
		cout << bestSolution[i] << " ";
	}
	cout << "coste: "<< bestCost << endl;

	return bestSolution;
}

int * Instance::simAnnealingGeometric(int * cost, ofstream &outfile){
	int costDiff,swap;
	*cost=0;
	int * DLB= new int[matrixSize];
	int * bestSolution= new int[matrixSize];
	int bestCost = 0;
	bool improve_flag;
	for(int i=0;i<matrixSize;i++){
		DLB[i]=0;
	}

	int* unitAndLocationAssociation = new int[matrixSize];

	/*First random solution*/
	for(int i=0; i<matrixSize; i++){

		unitAndLocationAssociation[i]= i+1;
	}
	random_shuffle(&unitAndLocationAssociation[0],&unitAndLocationAssociation[matrixSize]);
	//Calculation of the cost of the generated solution
	for(int i=0;i<matrixSize;i++){
		for(int j=0;j<matrixSize;j++){
			if(i!=j)
				*cost+=flowMatrix[i][j] * distanceMatrix[unitAndLocationAssociation[i]-1][unitAndLocationAssociation[j]-1];
		}
	}
	double initialT= (double)*cost * 1.5;
	double T = initialT;
	outfile << "Initial solution cost: " << *cost << ", Initial temperature: "<<initialT << endl;
	//Main loop
	int it = 0;
	while(it<50000 && checkDLB(DLB) && T>(5.0*initialT)/100.0){
		for(int i=0; i<matrixSize;i++){
			if(DLB[i]==0){
				improve_flag=false;
				for (int j=0;j<matrixSize; j++){
					if(j!=i && DLB[j]==0){
						costDiff = checkMove(unitAndLocationAssociation,i,j);
						it++;
						if(costDiff<0){
							//Making the change effective
							swap = unitAndLocationAssociation[i];
							unitAndLocationAssociation[i] = unitAndLocationAssociation[j];
							unitAndLocationAssociation[j] = swap;
							*cost+=costDiff;

							bestSolution = unitAndLocationAssociation;
							bestCost=*cost;
							DLB[i]=0;
							DLB[j]=0;
							improve_flag=true;
							outfile << "Movement in iteration: " << it << ", new cost: " << *cost << endl;

						}else{
							/*Simulated annealing*/
							double pAcceptance = exp(((double)costDiff)/T);
							if(T>pAcceptance){
								//Making the change effective
								swap = unitAndLocationAssociation[i];
								unitAndLocationAssociation[i] = unitAndLocationAssociation[j];
								unitAndLocationAssociation[j] = swap;
								*cost+=costDiff;
								DLB[i]=0;
								DLB[j]=0;
								outfile << "Worse movement in iteration: " << it << ", new cost: " << *cost << endl;

							}
						}

					}
					T= ALPHA * T ;
				}
				if (!improve_flag)
					DLB[i]=1;
			}

		}

	}
	if(it>50000)
		outfile << "Loop ended because of iteration number" <<endl;
	else if(!checkDLB(DLB))
		outfile << "Loop ended because of DLB"<< endl;
	else
		outfile << "Loop ended because of the temperature"<< endl;

	//Display solution and cost
	for(int i=0; i< matrixSize; i++){
		cout << bestSolution[i] << " ";
	}
	cout << "coste: "<< bestCost << endl;

	return bestSolution;
}

size_t Instance::split(const std::string &txt, std::vector<std::string> &strs, char ch)
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
