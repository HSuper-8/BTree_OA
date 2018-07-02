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
PAGE getPage(long int rrn, int MAX, fstream& Idx){
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
  	return page;
}

// Insere novo registro na pagina de forma ordenada
void insertInPage(PAGE page, string record, long int son, int MAX, fstream& Idx){ 
	int k;
	for (k = page->keyCount; (k > 0) && (record.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->childrenRRNs[k+1] = page->childrenRRNs[k];    
  	}
	page->keys[k].assign(record);
	page->childrenRRNs[k+1] = son;
	page->keyCount++;
	writePage(page, false, MAX, Idx);
}

// Escreve pagina no arquivo. Se for nova, escreve no fim do arquivo,
// senão, sobrescreve na posicao correspondente
void writePage(PAGE page, bool newPage, int MAX, fstream& Idx){
	int k;
    if(!newPage) Idx.seekp(page->RRN);
    else{
      Idx.close();
      Idx.open("../res/indicelista.bt", std::ios::app);
    }
    int pos = Idx.tellp();
    for(k = 0; k < MAX; k++)
      Idx << page->keys[k] << " ";

  	pos = Idx.tellp();
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
  
    if(newPage){
      Idx.close();
      Idx.open("../res/indicelista.bt");
    }
}

bool insertionProcess(string record, long int currentRRN, string& returnRecord, long int *returnRRN, int MAX, fstream& Idx){ 
	bool grownUp;
	long j, i = 0;

	if (currentRRN == -1){ 
  		returnRecord.assign(record);
  		(*returnRRN) = -1;
    	return true;
  	}

  	PAGE page = getPage(currentRRN, MAX, Idx);
  	i = position(page, record);

	if (binary_search(page->keys.begin(), page->keys.end(), record)){
		printf(" Erro: REGISTRO ja esta presente\n");
    	return false;
  	}

  	grownUp = insertionProcess(record, page->childrenRRNs[i], returnRecord, &(*returnRRN), MAX, Idx);
	if(!grownUp) return false;

 	if (page->keyCount < MAX){   //PAGINA tem espaco
		insertInPage(page, returnRecord, (*returnRRN), MAX, Idx);
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){ // novo registro vai para nova página
		insertInPage(new_Page, page->keys[MAX-1], page->childrenRRNs[MAX], MAX, Idx);
		page->keys[MAX-1].assign("                ");
		page->childrenRRNs[MAX] = -1;
		page->keyCount --;
		insertInPage(page, returnRecord, (*returnRRN), MAX, Idx);
	}
	else // registro é inserido na pagina corrente
		insertInPage(new_Page, returnRecord, (*returnRRN), MAX, Idx);

	for(j = MIN + 2; j <= MAX; j++){
		insertInPage(new_Page, page->keys[j-1], page->childrenRRNs[j], MAX, Idx);
		page->keys[j-1].assign("                ");
		page->childrenRRNs[j] = -1;
		page->keyCount --;
	}

	new_Page->childrenRRNs[0] = page->childrenRRNs[MIN + 1];
	returnRecord.assign(page->keys[MIN]);
	(*returnRRN) = new_Page->RRN;
	page->keys[MIN].assign("                ");
	page->childrenRRNs[MIN+1] = -1;
	page->keyCount = MIN;
	writePage(page, false, MAX, Idx); 
	writePage(new_Page, true, MAX, Idx);

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
long int insert(string record, long int rootRRN, int MAX, fstream& Idx){ 
	bool grownUp;
	string returnRecord = " ";
	long int returnRRN;
	grownUp = insertionProcess(record, rootRRN, returnRecord, &returnRRN, MAX, Idx);
	
	if (grownUp){  // Arvore cresce na altura pela raiz
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(returnRecord);
    	newRoot->childrenRRNs[0] = rootRRN;
    	newRoot->childrenRRNs[1] = returnRRN;
    	writePage(newRoot, true, MAX, Idx);
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
	fstream Idx("../res/indicelista.bt", std::fstream::in | std::fstream::out);
	long int rootRRN = -1, oldRRN;
	string line = "to begin", record, rrnRecordStr;

	while(line.size() != 0){ // Enquanto não for ultima linha do arquivo
		long int rrnRecord = List.tellg();
		rrnRecordStr = createRRN(rrnRecord);
		getline(List, line);
		record = CreateKey(line);

		string btreeLine = record + "|" + rrnRecordStr; // registros a serem inseridos
		oldRRN = rootRRN;
		rootRRN = insert(btreeLine, rootRRN, MAX, Idx);

		//if(oldRRN != rootRRN){
			//Idx.seekp(0);
			//cout << to_string(rootRRN) << endl;
			//Idx << to_string(rootRRN);
		//}
	}

	List.close();
	return rootRRN;
}