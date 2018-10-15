
#include "Instance.h"
#include "random.h"

int main(int argc,  char** argv) {
	if(argc<3){
		cout << "You need to specify the path to the data and the seed.";
		return 1;
	}
	std::srand(atoi(argv[2]));
	int *sol;
	int * cost= new int[1];
	Instance* i=new Instance(argv[1]);
	//i->display();
	//i->greedy(cost);
	sol=i->bestFirst(cost);
	//system("pause");
}
