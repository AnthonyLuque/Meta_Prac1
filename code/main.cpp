#include <time.h>
#include "Instance.h"

int main(int argc,  char** argv) {
	argc = 11;
	argv[1] = (char*)"datos/cnf02.dat";
	argv[2] = (char*)"datos/cnf01.dat";
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

	std::vector<const char*> seeds;
	seeds.push_back("12345678");
	seeds.push_back("23456781");
	seeds.push_back("34567812");
	seeds.push_back("45678123");
	seeds.push_back("56781234");


	for (int itFile = 1; itFile <= argc - 1; itFile++) {
		cout << "File:" << argv[itFile] << endl;
		Instance* instance = new Instance(argv[itFile]);

		for (int i = 0; i < seeds.size(); i++) {
			cout << "Seeds:" << seeds[i] << endl;
			std::srand(atoi(seeds[i]));

			// Greedy
			const clock_t greedy_begin_time = clock();
			instance->greedy(cost);
			std::cout << "Greedy execution time: " << float(clock() - greedy_begin_time) / CLOCKS_PER_SEC << endl;

			// Best First
			const clock_t bestFirst_begin_time = clock();
			instance->bestFirst(cost);
			std::cout << "Best First execution time: " << float(clock() - bestFirst_begin_time) / CLOCKS_PER_SEC << endl;

			// Simulated Annealing
			const clock_t simulatedAnnealingBoltzmann_begin_time = clock();
			instance->simAnnealingBoltzmann(cost);
			std::cout << "Simulated Annealing Boltzmann execution time: " << float(clock() - simulatedAnnealingBoltzmann_begin_time) / CLOCKS_PER_SEC << endl;

			const clock_t simulatedAnnealingGeometric_begin_time = clock();
			instance->simAnnealingGeometric(cost);
			std::cout << "Simulated Annealing Geometric execution time: " << float(clock() - simulatedAnnealingGeometric_begin_time) / CLOCKS_PER_SEC << endl;

			cout << endl;
		}
	
	}

	system("pause");
	return 0;
}
