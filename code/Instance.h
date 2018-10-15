/*
 * Instance.h
 *
 *  Created on: Oct 1, 2018
 *      Author: melthalas
 */

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#define ALPHA 0.9

using namespace std;

class Instance {

	int matrixSize;
	int** flowMatrix;
	int** distanceMatrix;

	size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
	bool checkDLB(int * DLB);
	int checkMove(int * sol, int i, int j);
public:
	Instance(string path);
	void display();
	int * greedy(int * cost);
	int * bestFirst(int * cost);
	int * simAnnealingBoltzmann(int * cost);
	int * simAnnealingGeometric(int * cost);
	virtual ~Instance();
};

#endif /* INSTANCE_H_ */
