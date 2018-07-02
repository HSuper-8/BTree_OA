#include "indice.h"

using namespace std;

int position(PAGE page, string compare){
	int i = 0;
	while(i < page->keyCount){
		if(compare.compare(page->keys[i]) < 0) break;
		i++;
	}
	return i;
}

void insertInPage(PAGE page, string record, long int son, fstream& Idx, int MAX){ 
	int k;
	for (k = page->keyCount; (k > 0) && (record.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->childrenRRNs[k+1] = page->childrenRRNs[k];    
  	}
	page->keys[k].assign(record);
	page->childrenRRNs[k+1] = son;
	page->keyCount++;
	writePage(page, Idx, false, MAX);
}

PAGE getPage(long int rrn, fstream& Idx, int MAX){
	PAGE page = newPage(MAX);
	Idx.seekp(rrn);
	string records, RRNs;
	int k = 0, i = 0;
	getline(Idx, records);
	while(k < records.size()){
		page->keys[i++].assign( records.substr(k, 16) );
    	page->keyCount ++;
    	k += 17;
  	}
  	getline(Idx, RRNs);
  	k = 0;
  	i = 0;
  	while(k < RRNs.size()){
    	page->childrenRRNs[i++] = stoi( RRNs.substr(k, 7) );
    	k += 8;
  	}
  	page->RRN = rrn;
  	return page;
}

bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, fstream& Idx, int MAX){ 
	bool grownUp;
	long j, i = 0;

	if (currentRRN == -1){ 
  		returnRecord.assign(record);
  		(*returnRRN) = -1;
    	return true;
  	}

  	PAGE page = getPage(currentRRN, Idx, MAX);
  	i = position(page, record);

	if (binary_search(page->keys.begin(), page->keys.end(), record)){
		printf(" Erro: REGISTRO ja esta presente\n");
    	return false;
  	}

  	grownUp = insertionProcess(record, page->childrenRRNs[i], returnRecord, &(*returnRRN), Idx, MAX);
	if(!grownUp) return false;

 	if (page->keyCount < MAX){   //PAGINA tem espaco
		insertInPage(page, returnRecord, (*returnRRN), Idx, MAX);
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){
		insertInPage(new_Page, page->keys[MAX-1], page->childrenRRNs[MAX], Idx, MAX);
		page->keys[MAX-1].assign("");
		page->childrenRRNs[MAX] = -1;
		page->keyCount --;
		insertInPage(page, returnRecord, (*returnRRN), Idx, MAX);
	}
	else
		insertInPage(new_Page, returnRecord, (*returnRRN), Idx, MAX);

	for(j = MIN + 2; j <= MAX; j++){
		insertInPage(new_Page, page->keys[j-1], page->childrenRRNs[j], Idx, MAX);
		page->keys[j-1].assign("");
		page->childrenRRNs[j] = -1;
		page->keyCount --;
	}

	new_Page->childrenRRNs[0] = page->childrenRRNs[MIN + 1];
	returnRecord.assign(page->keys[MIN]);
	(*returnRRN) = new_Page->RRN;
	page->keys[MIN].assign("");
	page->childrenRRNs[MIN+1] = -1;
	page->keyCount = MIN;
	writePage(page, Idx, false, MAX);
	writePage(new_Page, Idx, true, MAX);

  	return true;
}

void writePage(PAGE page, fstream& Idx, bool newPage, int MAX){
	int k;
	if(!newPage) Idx.seekg(page->RRN);
	else{
		string line;
		while(!Idx.eof()) getline(Idx, line);
		page->RRN = Idx.tellp();
	}

	for(k = 0; k < page->keyCount; k++)
		Idx << page->keys[k] << " ";
	if(k+1 < MAX){
		while(k+1 <= MAX) Idx << "         ";
	}
	Idx << "\n";

	for(k = 0; k < page->keyCount + 1; k++)
		Idx << createRRN(page->childrenRRNs[k]) << " ";
	if(k+1 < MAX+1){
		while(k+1 <= MAX+1) Idx << "        ";
	}
	Idx << "\n";
}

PAGE newPage(int MAX){
	PAGE page;
	page = new struct BTPAGE;
	page->keyCount = 0;
	for(int i = 0; i < MAX; i++) page->keys.push_back("");
	for(int i = 0; i <= MAX; i++) page->childrenRRNs.push_back(-1);
	return page;
}

// Retorna ponteiro para a pagina raiz
long int insert(string record, long int rootRRN, fstream& Idx, int MAX){ 
	bool grownUp;
	string returnRecord = "";
	long int *returnRRN;
	grownUp = insertionProcess(record, rootRRN, returnRecord, returnRRN, Idx, MAX);
	
	if (grownUp){  /* Arvore cresce na altura pela raiz */ 
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(returnRecord);
    	newRoot->childrenRRNs[0] = rootRRN;
    	newRoot->childrenRRNs[1] = (*returnRRN);
    	writePage(newRoot, Idx, true, MAX);
    	Idx.seekg(0);
    	Idx << createRRN(newRoot->RRN);
    	return newRoot->RRN;
	}
	return rootRRN;
}

string createRRN(long int rrn){
	string RRN;
	RRN.assign(to_string(rrn));
	if(RRN.size() < 7)
		while(RRN.size() != 7) RRN.push_back(' ');
	return RRN;
}

void printBTree(PAGE page, int level){ 
	/*if (page == NULL) return;
	cout << "Nível: " << level << endl;
	for (short i = 0; i < page->keys.size(); i++)
		cout << page->keys[i] << "|";
	cout << "\n" << endl;	
  	for (short i = 0; i <= page->keys.size(); i++)
    	printBTree(page->children[i], level+1);
    */
}

string CreateKey(string line){
	string record;
	record.push_back(toupper(line[0]));
	record.push_back(toupper(line[1]));
	record.push_back(toupper(line[2]));
	return record + line.substr(41, 5);
}

long int createBTree(int MAX){
	ifstream List("../res/lista.txt");
	fstream Idx("../res/indicelista.bt");
	long int rootRRN = -1;
	string line, record, rrnRecordStr;
	while(!List.eof()){
		long int rrnRecord = List.tellg();
		rrnRecordStr = createRRN(rrnRecord);
		getline(List, line);
		record = CreateKey(line);

		string btreeLine = record + "|" + rrnRecordStr;
		rootRRN = insert(btreeLine, rootRRN, Idx, MAX);
	}

	List.close();
	Idx.close();
}