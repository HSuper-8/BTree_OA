#include "indice.h"

using namespace std;

int position(PAGE page, string compare){ // posiçao em que novo registro devera ser inserido
	int i = 0;
	while(i < page->keyCount){
		if(compare.compare(page->keys[i]) < 0) break;
		i++;
	}
	return i;
}

// Constroi Pagina a partir leitura do arquivo
PAGE getPage(long int rrn, int MAX){
	fstream Idx;
	Idx.open("../res/indicelista.bt");
	PAGE page = newPage(MAX);
  	Idx.seekg(rrn);
  	string records, RRNs;
  	int k = 0, i = 0;
  	getline(Idx, records);
  	while(k < records.size()){
  		string str = "                ";
  		if(str.compare(records.substr(k, 16)) == 0) break;
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
  	Idx.close();
  	return page;
}

// Insere novo registro na pagina de forma ordenada
void insertInPage(PAGE page, string record, long int son, bool _write, int MAX){ 
	int k;
	for (k = page->keyCount; (k > 0) && (record.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->childrenRRNs[k+1] = page->childrenRRNs[k];    
  	}
	page->keys[k].assign(record);
	page->childrenRRNs[k+1] = son;
	page->keyCount++;
	if(_write) writePage(page, false, MAX);
}

// Escreve pagina no arquivo. Se for nova, escreve no fim do arquivo,
// senão, sobrescreve na posicao correspondente
void writePage(PAGE page, bool newPage, int MAX){
  int k;
  fstream Idx;
    if(!newPage){
      Idx.open("../res/indicelista.bt");
      Idx.seekp(page->RRN);
    }
    else{
      Idx.open("../res/indicelista.bt", std::ios::app);
    }

    for(k = 0; k < MAX; k++)
      Idx << page->keys[k] << " ";

  int pos = Idx.tellp();
    page->RRN = pos - (MAX * 17);
    Idx << "\n";

  for(k = 0; k < page->keyCount + 1; k++)
      Idx << createRRN(page->childrenRRNs[k]) << " ";
    if(k < MAX+1){
      while(k+1 <= MAX+1){
        Idx << createRRN(-1) << " ";
        k++;
      }
    }
  Idx << "\n";
  Idx.close();
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
		insertInPage(page, returnRecord, (*returnRRN), true, MAX);
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){ // novo registro é inserido na pagina corrente
		insertInPage(new_Page, page->keys[MAX-1], page->childrenRRNs[MAX], false, MAX);
		page->keys[MAX-1].assign("                ");
		page->childrenRRNs[MAX] = -1;
		page->keyCount --;
		insertInPage(page, returnRecord, (*returnRRN), false, MAX);
	}
	else // novo registro é inserido na nova pagina
		insertInPage(new_Page, returnRecord, (*returnRRN), false, MAX);

	for(j = MIN + 2; j <= MAX; j++){
		insertInPage(new_Page, page->keys[j-1], page->childrenRRNs[j], false, MAX);
		page->keys[j-1].assign("                ");
		page->childrenRRNs[j] = -1;
		page->keyCount --;
	}

	new_Page->childrenRRNs[0] = page->childrenRRNs[MIN + 1];
	returnRecord.assign(page->keys[MIN]);
	page->keys[MIN].assign("                ");
	page->childrenRRNs[MIN+1] = -1;
	page->keyCount = MIN;
	writePage(page, false, MAX);
	writePage(new_Page, true, MAX);
	(*returnRRN) = new_Page->RRN;

  	return true;
}

PAGE newPage(int MAX){ // Cria / Aloca nova pagina inicializando seus componentes
	PAGE page;
	page = new struct BTPAGE;
	page->keyCount = 0;
	for(int i = 0; i < MAX; i++) page->keys.push_back("                ");
	for(int i = 0; i <= MAX; i++) page->childrenRRNs.push_back(-1);
	return page;
}

// Retorna o RRN da pagina raiz
long int insert(string record, long int rootRRN, int MAX){ 
	bool grownUp;
	string returnRecord = " ";
	long int returnRRN;
	grownUp = insertionProcess(record, rootRRN, returnRecord, &returnRRN, MAX);
	
	if (grownUp){  // Arvore cresce na altura pela raiz
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(returnRecord);
    	newRoot->childrenRRNs[0] = rootRRN;
    	newRoot->childrenRRNs[1] = returnRRN;
    	writePage(newRoot, true, MAX);

    	fstream Idx;
    	Idx.open("../res/indicelista.bt");
    	Idx << to_string(newRoot->RRN);
    	Idx.close();
    	return newRoot->RRN;
	}
	return rootRRN;
}

string createRRN(long int rrn){ // cria string de tamanho fixo 7 com o RRN 
	string RRN;
	RRN.assign(to_string(rrn));
	if(RRN.size() < 7)
		while(RRN.size() != 7) RRN.push_back(' ');
	else 
		return (to_string(rrn)).substr(0, 7);
	return RRN;
}

void printBTree(PAGE page, int level){ 

}

// Cria a chave primária a partir de uma linha lida
string CreateKey(string line){
	string record;
	record.push_back(toupper(line[0]));
	record.push_back(toupper(line[1]));
	record.push_back(toupper(line[2]));
	return record + line.substr(41, 5);
}

long int createBTree(int MAX){
	ifstream List("../res/lista.txt"); // Arquivo original com os dados dos Alunos
	// Arquivo de Indices Primarios (Arvore B):
	//fstream Idx("../res/indicelista.bt", std::fstream::in | std::fstream::out);
	long int rootRRN = -1, oldRRN;
	string line = "to begin", record, rrnRecordStr;

	getline(List, line);
	while(line.size() != 0){ // Enquanto não for ultima linha do arquivo
		long int rrnRecord = List.tellg();
		rrnRecordStr = createRRN(rrnRecord);
		record = CreateKey(line);

		string btreeLine = record + "|" + rrnRecordStr; // registros a serem inseridos
		oldRRN = rootRRN;
		rootRRN = insert(btreeLine, rootRRN, MAX);
		getline(List, line);
	}

	List.close();
	return rootRRN;
}