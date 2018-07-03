#include "indice.h"

using namespace std;

int main(int argc, char *argv[]){ 
	string ordem;
	cout << "Digite a ordem da Árvore B: " << endl;
	getline(cin, ordem);
	int MAX = stoi(ordem) - 1;

	ofstream Idx("../res/indicelista.bt");
	Idx << "20                 " << endl; //Prepara arquivo para inserir arvore b
	Idx.close();

	createBTree(MAX);
	Menu(MAX);
	return 0;
}
