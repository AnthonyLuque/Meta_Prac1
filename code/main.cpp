
#include "Instance.h"

int main(int argc,  char** argv) {
	if(argc<3){
		cout << "You need to specify the path to the data and the seed.";
		return 1;
	}
	std::srand(atoi(argv[2]));
	int * cost= new int[1];
	Instance* i=new Instance(argv[1]);
	//i->display();
	i->greedy(cost);
	i->bestFirst(cost);
	i->simAnnealingBoltzmann(cost);
	i->simAnnealingGeometric(cost);
	//system("pause");
	return 0;
}
