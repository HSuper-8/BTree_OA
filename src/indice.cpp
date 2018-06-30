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

void insertInPage(PAGE page, string record, PAGE son){ 
	int k;
	for (k = page->keyCount; (k > 0) && (record.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->children[k+1] = page->children[k];    
  	}
	page->keys[k].assign(record);
	page->children[k+1] = son;
	page->keyCount++;
} 

bool insertionProcess(string record, PAGE page, string& returnRecord, PAGE *returnPage, int MAX){ 
	bool grownUp;
	long j, i = 0;
	PAGE pageTemp;

	if (page == NULL){ 
  		returnRecord.assign(record);
  		(*returnPage) = NULL;
    	return true;
  	}

  	i = position(page, record);

	if (binary_search(page->keys.begin(), page->keys.end(), record)){
		printf(" Erro: REGISTRO ja esta presente\n");
    	return false;
  	}

	grownUp = insertionProcess(record, page->children[i], returnRecord, returnPage, MAX);
	if(!grownUp) return false;

 	if (page->keyCount < MAX){   //PAGINA tem espaco
		insertInPage(page, returnRecord, (*returnPage));
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){
		insertInPage(new_Page, page->keys[MAX-1], page->children[MAX]);
		page->keys[MAX-1].assign("");
		page->children[MAX] = NULL;
		page->keyCount --;
		insertInPage(page, returnRecord, (*returnPage));
	}
	else
		insertInPage(new_Page, returnRecord, (*returnPage));

	for(j = MIN + 2; j <= MAX; j++){
		insertInPage(new_Page, page->keys[j-1], page->children[j]);
		page->keys[j-1].assign("");
		page->children[j] = NULL;
		page->keyCount --;
	}

	new_Page->children[0] = page->children[MIN + 1];
	returnRecord.assign(page->keys[MIN]);
	(*returnPage) = new_Page;
	page->keys[MIN].assign("");
	page->children[MIN+1] = NULL;
	page->keyCount = MIN;

  	return true;
}

PAGE newPage(int MAX){
	PAGE page;
	page = new struct BTPAGE;
	page->keyCount = 0;
	for(int i = 0; i < MAX; i++) page->keys.push_back("");
	for(int i = 0; i <= MAX; i++) page->children.push_back(NULL);
	return page;
}

// Retorna ponteiro para a pagina raiz
PAGE insert(string record, PAGE pageRoot, int MAX){ 
	bool grownUp;
	string returnRecord = "";
	PAGE returnPage;
	grownUp = insertionProcess(record, pageRoot, returnRecord, &returnPage, MAX);
	
	if (grownUp){  /* Arvore cresce na altura pela raiz */ 
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(returnRecord);
    	newRoot->children[0] = pageRoot;
    	newRoot->children[1] = returnPage;
    	return newRoot;
	}
	return pageRoot;
}

void printBTree(PAGE page, int level){ 
	if (page == NULL) return;
	cout << "Nível: " << level << endl;
	for (short i = 0; i < page->keys.size(); i++)
		cout << page->keys[i] << "|";
	cout << "\n" << endl;	
  	for (short i = 0; i <= page->keys.size(); i++)
    	printBTree(page->children[i], level+1);
} 