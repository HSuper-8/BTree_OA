#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef struct BTPAGE* PAGE;

struct BTPAGE {
	int RRN;
	short keyCount;
	vector<string> keys;
	vector<PAGE> children;
	PAGE pageDad;
	//std::vector<short> RRNchildren;
};

int position(PAGE page, string compare);
void insertInPage(PAGE page, string record, PAGE son);
bool insertionProcess(string record, PAGE page, string &returnRecord, PAGE *returnPage, int MAX);
PAGE newPage(int MAX);
PAGE insert(string record, PAGE pageRoot, int MAX);
void printBTree(PAGE page, int level);