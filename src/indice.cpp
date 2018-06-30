#include "indice.h"

using namespace std;

int position(vector<string> keys, string compare){
	int i = 0;
	while(i < keys.size()){
		if(compare.compare(keys[i]) < 0) break;
		i++;
	}
	return i;
}

void insere_na_pagina(PAGE page, string reg, PAGE son){ 
	int k;
	for (k = page->keyCount; (k > 0) && (reg.compare(page->keys[k-1]) < 0); k--){
    	page->keys[k].assign(page->keys[k-1]);
    	page->children[k+1] = page->children[k];    
  	}
	page->keys[k].assign(reg);
	page->children[k+1] = son;
	page->keyCount++;
} 

bool insere_rec(string reg, PAGE page, string& reg_retorno, PAGE *page_retorno, int MAX){ 
	bool grownUp;
	long j, i = 0;
	PAGE pageTemp;

	if (page == NULL){ 
  		reg_retorno.assign(reg);
  		(*page_retorno) = NULL;
    	return true;
  	}

  	i = position(page->keys, reg);

	if (binary_search (page->keys.begin(), page->keys.end(), reg)){
		printf(" Erro: REGISTRO ja esta presente\n");
    	return false;
  	}

	grownUp = insere_rec(reg, page->children[i], reg_retorno, page_retorno, MAX);
	if(!grownUp) return false;

 	if (page->keyCount < MAX){   /* PAGINA tem espaco */
		insere_na_pagina(page, reg_retorno, (*page_retorno));
		return false;
	}
  
	//Overflow: PAGINA tem que ser dividida
	PAGE new_Page = newPage(MAX);
	int MIN = MAX/2;

	if (i < MIN + 1){
		insere_na_pagina(new_Page, page->keys[MAX-1], page->children[MAX]);
		page->keys[MAX-1].assign("");
		page->keyCount --;
		insere_na_pagina(page, reg_retorno, (*page_retorno));
	}
	else
		insere_na_pagina(new_Page, reg_retorno, (*page_retorno));

	for(j = MIN + 2; j <= MAX; j++){
		insere_na_pagina(new_Page, page->keys[j-1], page->children[j]);
		page->keys[j-1].assign("");
		page->children[j] = NULL;
		page->keyCount --;
	}

	new_Page->children.insert(new_Page->children.begin(), page->children[MIN + 1]);
	reg_retorno.assign(page->keys[MIN]);
	(*page_retorno) = new_Page;
	page->keys[MIN].assign("");
	page->keyCount --;

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
PAGE insere(string reg, PAGE pageRoot, int MAX){ 
	bool grownUp;
	string reg_retorno = "";
	PAGE page_retorno;
	grownUp = insere_rec(reg, pageRoot, reg_retorno, &page_retorno, MAX);
	
	if (grownUp){  /* Arvore cresce na altura pela raiz */ 
    	PAGE newRoot = newPage(MAX);
    	newRoot->keyCount ++;
    	newRoot->keys[0].assign(reg_retorno);
    	newRoot->children[0] = pageRoot;
    	newRoot->children[1] = page_retorno;
    	return newRoot;
	}
	return pageRoot;
}

void imprime(PAGE p, int nivel){ 
	if (p == NULL) return;
	cout << "Nivel: " << nivel << endl;
	for (short i = 0; i < p->keyCount; i++)
		cout << p->keys[i] << "|";
	cout << "\n" << endl;	
  	for (short i = 0; i <= p->keyCount; i++)
    	imprime(p->children[i], nivel+1);
} 