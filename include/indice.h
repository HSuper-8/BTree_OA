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
void printBTree(PAGE p, int level);
long int createBTree(int MAX);

// Search record
bool search(long int RRN, string KEY, int *FOUND_RRN, int *FOUND_POS, int MAX, int *seeks);
bool find(PAGE page, string key, int *i);
void choice_key(int MAX);