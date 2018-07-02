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
	//std::vector<short> RRNchildren;
};

int position(PAGE page, string compare);
void insertInPage(PAGE page, string record, long int son, fstream& Idx, int MAX);
PAGE getPage(long int rrn, fstream& Idx, int MAX);
string createRRN(long int rrn);
PAGE newPage(int MAX);

bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, fstream& Idx, int MAX);
void writePage(PAGE page, fstream& Idx, bool newPage, int MAX);

long int insert(string record, long int rootRRN, fstream& Idx, int MAX);
void printBTree(PAGE p, int level);
long int createBTree(int MAX);