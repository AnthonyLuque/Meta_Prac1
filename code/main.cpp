#include <time.h>
#include "Instance.h"

int main(int argc,  char** argv) {
	argc = 11;
	argv[1] = (char*)"datos/cnf01.dat";
	argv[2] = (char*)"datos/cnf02.dat";
	argv[3] = (char*)"datos/cnf03.dat";
	argv[4] = (char*)"datos/cnf04dat.sec";
	argv[5] = (char*)"datos/cnf05dat.sec";
	argv[6] = (char*)"datos/cnf06.dat";
	argv[7] = (char*)"datos/cnf07.dat";
	argv[8] = (char*)"datos/cnf08.dat";
	argv[9] = (char*)"datos/cnf09.dat";
	argv[10] = (char*)"datos/cnf10.dat";
	
	if(argc<2){
		cout << "You need to specify the path of the data files.";
		return 1;
	}

	int * cost = new int[1];
	//long greedyAlgorithmValues;
	long bestFirstAlgorithmValues[5];
	long saBoltzmannAlgorithmValues[5];
	long saGeometricAlgorithmValues[5];
	std::vector <long> bestValues;
		bestValues.push_back(6156);
		bestValues.push_back(64);
		bestValues.push_back(6922);
		bestValues.push_back(48816);
		bestValues.push_back(64);
		bestValues.push_back(21052466);
		bestValues.push_back(1185996137);
		bestValues.push_back(498896643);
		bestValues.push_back(44759294);
		bestValues.push_back(8133398);

	std::vector<const char*> seeds;
	seeds.push_back("77374129");
	seeds.push_back("73741297");
	seeds.push_back("37412977");
	seeds.push_back("74129773");
	seeds.push_back("41297737");


	for (int itFile = 1; itFile <= argc - 1; itFile++) {
		cout << "File:" << argv[itFile] << endl;
		Instance* instance = new Instance(argv[itFile]);

		// Greedy
		const clock_t greedy_begin_time = clock();
		instance->greedy(cost);
		std::cout << "Greedy execution time: " << float(clock() - greedy_begin_time) / CLOCKS_PER_SEC << endl;
		//greedyAlgorithmValues = *cost;

		for (int i = 0; i < seeds.size(); i++) {
			cout << "Seeds:" << seeds[i] << endl;
			std::srand(atoi(seeds[i]));

			// Best First
			ofstream outfile;
			std::vector<string> v;
			instance->split(argv[itFile],v,'/');
			outfile.open("logs/BF" + v.at(1) + seeds[i] + ".txt");
			const clock_t bestFirst_begin_time = clock();
			instance->bestFirst(cost, outfile);
			std::cout << "Best First execution time: " << float(clock() - bestFirst_begin_time) / CLOCKS_PER_SEC << endl;
			outfile.close();
			bestFirstAlgorithmValues[i] = *cost;

			// Simulated Annealing
			instance->split(argv[itFile],v,'/');
			outfile.open("logs/SAB" + v.at(1) + seeds[i] + ".txt");
			const clock_t simulatedAnnealingBoltzmann_begin_time = clock();
			instance->simAnnealingBoltzmann(cost, outfile);
			std::cout << "Simulated Annealing Boltzmann execution time: " << float(clock() - simulatedAnnealingBoltzmann_begin_time) / CLOCKS_PER_SEC << endl;
			outfile.close();
			saBoltzmannAlgorithmValues[i] = *cost;

			instance->split(argv[itFile],v,'/');
			outfile.open("logs/SAG" + v.at(1) + seeds[i] + ".txt");
			const clock_t simulatedAnnealingGeometric_begin_time = clock();
			instance->simAnnealingGeometric(cost, outfile);
			std::cout << "Simulated Annealing Geometric execution time: " << float(clock() - simulatedAnnealingGeometric_begin_time) / CLOCKS_PER_SEC << endl;
			outfile.close();
			saGeometricAlgorithmValues[i] = *cost;


			cout << endl;
		}
		// Deviations calcul

				/*long desvGreedy = 0;
				for (long value : greedyAlgorithmValues) {
					desvGreedy += 100 * (value - bestValues[itFile - 1]) / bestValues[itFile - 1];
				}
				desvGreedy /= 5;
				cout << "Greedy Deviation: " << desvGreedy << endl;*/

				long desvBestFirst = 0;
				for (long value : bestFirstAlgorithmValues) {
					desvBestFirst += 100 * (value - bestValues[itFile - 1]) / bestValues[itFile - 1];
				}
				desvBestFirst /= 5;
				cout << "Best First Deviation: " << desvBestFirst << endl;

				long desvSaBoltzmann = 0;
				for (long value : saBoltzmannAlgorithmValues) {
					desvSaBoltzmann += 100 * (value - bestValues[itFile - 1]) / bestValues[itFile - 1];
				}
				desvSaBoltzmann /= 5;
				cout << "Boltzmann Simulated Annealing Deviation: " << desvSaBoltzmann << endl;

				long desvSaGeometric = 0;
				for (long value : saGeometricAlgorithmValues) {
					desvSaGeometric += 100 * (value - bestValues[itFile - 1]) / bestValues[itFile - 1];
				}
				desvSaGeometric /= 5;
				cout << "Geometric Simulated Annealing Deviation: " << desvSaGeometric << endl;

			}
	//system("pause");
		return 0;
	}
	



