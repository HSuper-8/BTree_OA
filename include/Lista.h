#ifndef LISTA_H					
#define LISTA_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <tuple>

//Estrutura para armazenar e tratar as chaves primarias

struct Chaves {
	std::string chave; 
	std::string curso;
	std::string RRN;
	std::string matricula; 
	unsigned indice;
	bool MarcadorAtivo;
	Chaves *prox;
	Chaves *anterior;
};

class Lista{
public:
	Chaves* Inicio;
};


class Arquivos{
public:
	void Menu(Lista* lista);
	void Arquivo(Lista *lista);
	void LerListaOriginal(Lista* lista, unsigned *indice, std::istream& Lista, std::ostream& indicelista, Chaves **no, Chaves *anterior);
	void Inserir(unsigned *indice, Chaves **no, Chaves *anterior, std::string chave, std::string RRN, std::string curso, std::ostream& indicelista);	
	void ImprimeLista();

};

 

#endif //LISTA_H