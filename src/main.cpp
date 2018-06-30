#include "indice.h"
//#include "heapsort.h"
using namespace std;

int main(int argc, char *argv[]){ 
	if(argc < 2){
		cout << "\nError! Please enter a parameter (tree order)\n" << endl;
		return 1;
	}

	char *ordem = argv[1];
	string ord = string(ordem);
	int MAX = stoi(ord) - 1;
	
	string key;
	PAGE pageRoot = NULL;
	cout << "insira chave: ";
	getline(cin, key);
	while(true){
    	pageRoot = insert(key, pageRoot, MAX);
    	printBTree(pageRoot, 0);
    	cout << "insira chave: ";
		getline(cin, key);
	}
	return 0;
}

// C G J X N S U O A E B H I F K L Q R T V U W Z