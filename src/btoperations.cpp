#include "btoperations.h"

using namespace std;

bool Delete(string record, long int rrnCurrent, bool decreased, int MAX) {
	int i, j;
	PAGE page;
	if (rrnCurrent == -1) {
		cout << "\nErro: registro nao esta na arvore.\n" << endl;
		return false;
	}

	page = getPage(*rrnCurrent);
	i = position(page, record);
	if (record.compare( (page->keys[i-1])).substr(0, 8) == 0) {
		if (page->childrenRRNs[i] == -1) { // mudei de i-1 para i
			page->keyCount --;
			for (j = i; j <= page->keyCount; j++) {
				page->keys[j-1].assign(page->keys[j]);
				page->childrenRRNs[j] = page->childrenRRNs[j+1];
			}
			page->keys[i].assign("*               ");
			write(page, false, MAX);
			return(page->keyCount < (MAX/2));
		}

		decreased = antecessor(rrnCurrent, i, page->childrenRRNs[i-1], decreased, MAX);
		if(decreased)
			decreased = reconstituir(page->childrenRRNs[i-1], *rrnCurrent, i-1, decreased, MAX);
		return decreased;
	}
	if(record.compare( (page->keys[i-1])).substr(0, 8) > 0) i++;
	decreased = excluirAux(record, &page->childrenRRNs[i-1], decreased);
	if(decreased)
		decreased = reconstituir(page->childrenRRNs[i-1], rrnCurrent, i-1, decreased, MAX);
	decreased = reconstituir(page->childrenRRNs[i-1], rrnCurrent, i-1, decreased, MAX);
	return decreased;
}

bool excluirAux(string record, long int rrnCurrent, bool decreased, int MAX){
	int i, j;
	PAGE page;
	if (rrnCurrent == -1) {
		cout << "\nErro: registro nao esta na arvore.\n") << endl;
		return false;
	}
	page = getPage(rrnCurrent, MAX);
	i = 1;
	i = position(PAGE, record);
	if(record.compare(page->keys[i - 1]) == 0) {
		if(page->childrenRRNs[i-1] == -1) {
			page->keyCount--;
			for (j = i; j <= page->keyCount; j++) {
				page->keys[j - 1].assign(page->keys[j]);
				page->childrenRRNs[j] = page->childrenRRNs[j + 1];
			}
			return (page->keyCount < (MAX/2));
		}
		decreased = antecessor(page, i, page->childrenRRNs[i-1], decreased, MAX);
		if(decreased)
			decreased = reconstituir(page->childrenRRNs[i - 1], page, i - 1, decreased, MAX);
		return decreased;
	}
	if(record.compare(page->keys[i-1]) > 0) i++;
	decreased = excluirAux(record, page->childrenRRNs[i-1], decreased, MAX);
	if(*decreased)
	decreased = reconstituir(page->childrenRRNs[i-1], page, i-1, decreased);
	decreased = reconstituir(page->childrenRRNs[i-1], page, i-1, decreased);
	return decreased;
}

bool antecessor(long int rrnCurrent, int i, long int rrnDad, bool decreased, int MAX){
	PAGE dad = getPage(rrnDad, MAX);
	if (dad->childrenRRNs[dad->keyCount] != -1){
		decreased = antecessor((*rrnCurrent), i, dad->childrenRRNs[dad->keyCount], decreased);
		if(decreased)
			decreased = reconstituir(dad->childrenRRNs[dad->keyCount], dad, dad->keyCount, decreased, MAX);
		return decreased;
	}
	ap->keys[i-1].assign(dad->keys[dad->keyCount-1]);
	dad->keyCount--;
	return(dad->keyCount < (MAX/2));
}

bool reconstituir(long int rrn, PAGE dad, int posPai, bool diminuiu, int MAX){
	long int rrnAux;
	int dispAux, j;
	if (posPai < dad->keyCount){
		rrnAux = dad->childrenRRNs[posPai + 1];
		pageCurrent = getPage(rrn, MAX);
		dispAux = (rrnAux->keyCount - (MAX/2) + 1) / 2;
		pageCurrent->keys[pageCurrent->keyCount].assign(dad->keys[posPai]);
		pageCurrent->childrenRRNs[pageCurrent->keyCount + 1] = pageCurrent->childrenRRNs[0];
		pageCurrent->keyCount++;
		if (dispAux > 0) {
			for (j = 1; j < dispAux; j++)
				insertInPage(pageCurrent, pageCurrent->keys[j-1], pageCurrent->childrenRRNs[j], false, MAX);
			dad->keys[posPai].assign(pageCurrent->keys[dispAux - 1]);
			pageCurrent->keyCount -= dispAux;
			for (j = 0; j < pageCurrent->keyCount; j++)
				pageCurrent->keys[j].assign(pageCurrent->keys[j + dispAux]);
			for (j = 0; j <= pageCurrent->keyCount; j++)
				pageCurrent->childrenRRNs[j] = pageCurrent->childrenRRNs[j + dispAux];
			diminuiu = false;
		}
		else {
			for (j = 1; j <= (MAX/2); j++)
				insertInPage(pageCurrent, pageCurrent->keys[j-1], pageCurrent->childrenRRNs[j], false, MAX);
			pageCurrent = newPage(MAX);
			pageCurrent->RRN = rrnCurrent;
			write(pageCurrent, false, MAX);
			for (j = posPai + 1; j < dad->keyCount; j++){
				dad->keys[j - 1].assign(dad->keys[j]);
				dad->childrenRRNs[j] = dad->childrenRRNs[j+1];
			}
			dad->keyCount--;
			if (dad->keyCount >= (MAX/2))
				diminuiu = false;
		}
	}
	else{
		rrnAux = dad->childrenRRNs[posPai - 1];
		pageCurrent = getPage(rrnAux, MAX);
		dispAux = (pageCurrent->keyCount - (MAX/2) + 1) / 2;
		for(j = dad->keyCount; j >= 1; j--)
			dad->keys[j].assign(dad->keys[j - 1]);
		dad->keys[0].assign(dad->keys[posPai - 1]);
		for(j = dad->keyCount; j >= 0; j--)
			dad->childrenRRNs[j+1] = dad->childrenRRNs[j];
		dad->keyCount++;
		if(dispAux > 0) {
			for (j = 1; j < dispAux; j++)
				insertInPage(dad, pageCurrent->keys[pageCurrent->keyCount - j], pageCurrent->childrenRRNs[pageCurrent->keyCount - j + 1], false, MAX);
			dad->childrenRRNs[0] = pageCurrent->childrenRRNs[pageCurrent->keyCount - dispAux + 1];
			dad->keys[posPai - 1] = pageCurrent->keys[pageCurrent->keyCount - dispAux];
			pageCurrent->keyCount -= dispAux;
			diminuiu = false;
		}
		else {
			for (j = 1; j <= (MAX/2); j++)
				insertInPage(pageCurrent, dad->keys[j-1], dad->childrenRRNs[j], false, MAX);
						pageCurrent = newPage(MAX);
			int rrnDad = dad->RRN;
			dad = newPage(MAX);
			dad->RRN = rrnDad;
			write(dad, false, MAX);
			dad->keyCount--;
			if (dad->keyCount >= (MAX/2))
				decreased = false;
		}
	}
	return decreased;
}

void deleteKey(int MAX){
	cout << "digite a chave que queira excluir no formato do arquivo.txt" << endl;
	string key, record, rrn;
	long int rootRRN;
	getline(cin, key);
	record = creatKey(key);
	fstream Idx("../res/indicelista.bt");
	getline(Idx, rrn);
	rootRRN = stoi(rrn);


	bool decreased;
	decreased = Delete(record, &rootRRN, &decreased, MAX);
	PAGE root = getPage(rootRRN, MAX);
	if (decreased && root->keyCount == 0){
		root = getPage(root->childrenRRNs[0]);
		write(page, false, MAX);
		free(aux);
	}
}

