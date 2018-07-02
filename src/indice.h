#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ctype.h>

using namespace std;

typedef struct BTPAGE* PAGE;

struct BTPAGE {
	int RRN;
	short keyCount;
	vector<string> keys;
	vector<long int> childrenRRNs;
};

int position(PAGE page, string compare);
void writePage(PAGE page, bool newPage, int MAX, fstream& Idx);
void insertInPage(PAGE page, string record, long int son, int MAX, fstream& Idx);
PAGE getPage(long int rrn, int MAX, fstream& Idx);
string createRRN(long int rrn);
PAGE newPage(int MAX);
void changeRoot(PAGE newRoot);

bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, int MAX, fstream& Idx);

long int insert(string record, long int rootRRN, int MAX, fstream& Idx);
void printBTree(PAGE p, int level);
long int createBTree(int MAX);