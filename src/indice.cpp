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

void insertInPage(PAGE page, string record, long int son, int MAX){ 
	int k;
	for (k = page->keyCount; (k > 0) && (record.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->childrenRRNs[k+1] = page->childrenRRNs[k];    
  	}
	page->keys[k].assign(record);
	page->childrenRRNs[k+1] = son;
	page->keyCount++;
	writePage(page, false, MAX);
}

PAGE getPage(long int rrn, int MAX){
	ifstream Index("../res/indicelista.bt");
	PAGE page = newPage(MAX);
	Index.seekg(rrn);
	string records, RRNs;
	int k = 0, i = 0;
	getline(Index, records);
	while(k < records.size()){
		string str = "                ";
		if( str.compare(records.substr(k, 16)) == 0) break;
		page->keys[i++].assign( records.substr(k, 16) );
    	page->keyCount ++;
    	k += 17;
  	}
  	getline(Index, RRNs);
  	k = 0;
  	i = 0;
  	while(k < RRNs.size()){
    	page->childrenRRNs[i++] = stoi( RRNs.substr(k, 7) );
    	k += 8;
  	}
  	page->RRN = rrn;
  	Index.close();
  	return page;
}

bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, int MAX){ 
	bool grownUp;
	long j, i = 0;

	if (currentRRN == -1){ 
  		returnRecord.assign(record);
  		(*returnRRN) = -1;
    	return true;
  	}

  	PAGE page = getPage(currentRRN, MAX);
  	i = position(page, record);

	if (binary_search(page->keys.begin(), page->keys.end(), record)){
		printf(" Erro: REGISTRO ja esta presente\n");
    	return false;
  	}

  	grownUp = insertionProcess(record, page->childrenRRNs[i], returnRecord, &(*returnRRN), MAX);
	if(!grownUp) return false;

 	if (page->keyCount < MAX){   //PAGINA tem espaco
		insertInPage(page, returnRecord, (*returnRRN), MAX);
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){
		insertInPage(new_Page, page->keys[MAX-1], page->childrenRRNs[MAX], MAX);
		page->keys[MAX-1].assign("");
		page->childrenRRNs[MAX] = -1;
		page->keyCount --;
		insertInPage(page, returnRecord, (*returnRRN), MAX);
	}
	else
		insertInPage(new_Page, returnRecord, (*returnRRN), MAX);

	for(j = MIN + 2; j <= MAX; j++){
		insertInPage(new_Page, page->keys[j-1], page->childrenRRNs[j], MAX);
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
	writePage(page, false, MAX);
	writePage(new_Page, true, MAX);

  	return true;
}

void writePage(PAGE page, bool newPage, int MAX){
	int k;
	ofstream File;
	if(!newPage){
		File.open("../res/indicelista.bt");
		File.seekp(page->RRN);
	}
	else
		File.open("../res/indicelista.bt", ofstream::out | std::ofstream::app);

	for(k = 0; k < MAX; k++)
		File << page->keys[k] << " ";

	int pos = File.tellp();
	page->RRN = pos - (MAX * 17);
	File << "\n";

	for(k = 0; k < page->keyCount + 1; k++)
		File << createRRN(page->childrenRRNs[k]) << " ";
	if(k <= MAX+1){
		while(k+1 <= MAX+1){
			File << "-1     " << " ";
			k++;
		}
	}
	File << "\n";
	
	File.close();
}

PAGE newPage(int MAX){
	PAGE page;
	page = new struct BTPAGE;
	page->keyCount = 0;
	for(int i = 0; i < MAX; i++) page->keys.push_back("                ");
	for(int i = 0; i <= MAX; i++) page->childrenRRNs.push_back(-1);
	return page;
}

// Retorna ponteiro para a pagina raiz
long int insert(string record, long int rootRRN, int MAX){ 
	bool grownUp;
	string returnRecord = "                ";
	long int returnRRN;
	grownUp = insertionProcess(record, rootRRN, returnRecord, &returnRRN, MAX);
	
	if (grownUp){  /* Arvore cresce na altura pela raiz */ 
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(returnRecord);
    	newRoot->childrenRRNs[0] = rootRRN;
    	newRoot->childrenRRNs[1] = returnRRN;
    	writePage(newRoot, true, MAX);
    	
    	ofstream Idx("../res/indicelista.bt");
    	Idx.seekp(0);
    	Idx << createRRN(newRoot->RRN);
    	Idx.close();

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
	ofstream Idxx("../res/indicelista.bt");
	Idxx << "8      " << endl;
	Idxx.close();
	long int rootRRN = -1;
	string line = "to begin", record, rrnRecordStr;

	while(line.size() != 0){
		long int rrnRecord = List.tellg();
		rrnRecordStr = createRRN(rrnRecord);
		getline(List, line);
		record = CreateKey(line);

		string btreeLine = record + "|" + rrnRecordStr;
		rootRRN = insert(btreeLine, rootRRN, MAX);
	}

	List.close();
}