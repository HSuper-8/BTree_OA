#include "indice.h"

using namespace std;

int main(int argc, char *argv[]){ 
	if(argc < 2){
		cout << "\nError! Please enter a parameter (tree order)\n" << endl;
		return 1;
	}

	char *ordem = argv[1];
	int MAX = stoi(string(ordem)) - 1;
	
	ofstream Idx("../res/indicelista.bt");
	Idx << "20                 " << endl;
	Idx.close();

	createBTree(MAX);
	return 0;
}

// C G J X N S U O A E B H I F K L Q R T V W Z