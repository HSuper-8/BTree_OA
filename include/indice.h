#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ctype.h>
#include <iomanip>

using namespace std;

typedef struct BTPAGE* PAGE;

struct BTPAGE {
	int RRN;
	short keyCount; // contador de chaves existentes na página
	vector<string> keys; // vetor que contém as chaves
	vector<long int> childrenRRNs; // vetor que contém os rrn das páginas filhas
};

void Menu(int MAX);
string CreateKey(string line);
int position(PAGE page, string compare);
void writePage(PAGE page, bool newPage, int MAX);
void insertInPage(PAGE page, string record, long int son, bool _write, int MAX);
PAGE getPage(long int rrn, int MAX);
string createRRN(long int rrn);
PAGE newPage(int MAX);
void changeRoot(PAGE newRoot);
bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, int MAX);

long int insert(string record, long int rootRRN, int MAX);
void printBTree(long int rrn, int MAX, int nivel);
long int createBTree(int MAX);
void insertRecord(int MAX);

// Funções para busca
bool search(long int RRN, string KEY, int *FOUND_RRN, int *FOUND_POS, int MAX, int *seeks);
bool find(PAGE page, string key, int *i);
void choice_key(int MAX);
