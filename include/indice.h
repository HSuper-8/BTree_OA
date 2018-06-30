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

int position(vector<string> keys, string compare);
void insere_na_pagina(PAGE page, string reg, PAGE son);
bool insere_rec(string reg, PAGE page, string *reg_retorno, PAGE page_retorno, int MAX);
PAGE newPage(int MAX);
PAGE insere(string reg, PAGE pageRoot, int MAX);
void imprime(PAGE p, int nivel);