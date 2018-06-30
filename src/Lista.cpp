#include "Lista.h"

using namespace std;

void Arquivos::Menu(Lista* lista){
   string opcao;
   int op;
   while(true){
      cout <<endl<<endl;
      cout <<"\t\t**************************************************"<<endl;
      cout <<"\t\t"<<"*"<<setw(17)<<""<<setw(13)<<"MENU INDICES"<<setw(18)<<""<<"*"<<endl;
      cout <<"\t\t**************************************************"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"OPÇÃO:"<<"\t\t"<<"DESCRIÇÃO:"<<"\t\t"<<" *"<<endl;
      cout <<"\t\t"<<"*""------------------------------------------------"<<"*"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"[1]"<<"\t"<<"Mostrar LISTAS"<<"\t\t\t"<<" *"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"[2]"<<"\t"<<"Incluir REGRISTRO"<<"\t\t"<<" *"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"[3]"<<"\t"<<"Excluir REGISTRO"<<"\t\t"<<" *"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"[4]"<<"\t"<<"Buscar REGISTRO"<<"\t\t\t"<<" *"<<endl;
      cout <<"\t\t"<<"*"<<"\t"<<"[0]"<<"\t"<<"SAIR"<<"\t\t\t\t"<<" *"<<endl;
      cout <<"\t\t**************************************************"<<endl;
      cout <<"\t\t"<<setw(10)<<"Digite sua opção---> ";
      getline(cin, opcao);
      op = stoi(opcao);
    switch(op){
      case 1: 
         this->ImprimeLista();
         break;
      /*case 2:
         this->InserirRegistro();
         break;
      case 3:
         this->ExcluirRegistro();
         break;
      case 4:
         this->BuscaRegistro();
         break;*/
      case 0: 
         cout <<"\n\tVocê saiu do programa!\n";
         exit(0);
         break;
      default:
         cout <<"\n\tDigite uma opção válida!\n";
    }
  }
}

void Arquivos::Arquivo(Lista *lista){
   std::ifstream Lista("../res/lista.txt");
   std::ofstream indicelista("../res/indicelista.ind");

   // Ler Lista e cria indicelista
   unsigned indice = 0;
   LerListaOriginal(lista, &indice, Lista, indicelista, &lista->Inicio, NULL);

   
   Lista.close();
   indicelista.close();
}

void Arquivos::Inserir(unsigned *indice, Chaves **no, Chaves *anterior, std::string chave, string RRN, std::string curso, std::ostream& indicelista){
   (*no) = new Chaves;
   (*no)->chave = chave;
   (*no)->RRN = RRN;
   (*no)->MarcadorAtivo = 1;
   (*no)->prox = NULL;
   (*no)->indice = (*indice);
   (*no)->anterior = anterior;
   (*no)->curso = curso;
  
   indicelista << chave << "|" << (*no)->RRN << std::endl; 
}

void Arquivos::LerListaOriginal(Lista* lista, unsigned *indice, std::istream& Lista, std::ostream& indicelista, 
	Chaves **no, Chaves *anterior){

	if(!Lista.eof()){
		std::string linha, chave, curso, matricula;
		int rrn = Lista.tellg();
		getline(Lista, linha);

		unsigned contador = 0, espacos = 0;

		while( (chave.size() < 3) && (espacos <= 5) ){
			if( !isspace(linha[contador]) ){
				chave.push_back(linha[contador]);
			}
			else espacos++;
			contador++;
		}

		if(linha.size() != 0){
			curso.push_back(linha[52]);
			curso.push_back(linha[53]);         

			if(chave.size() < 3){
				while(chave.size() < 3){
					chave.push_back(' ');
				}
			}
			string RRN = to_string(rrn);

			if(RRN.size() < 9){
				while(RRN.size() < 9){
					RRN.push_back(' ');
				}
			}         

			// Passando para maiúsculas
			int i;
			for (i = 0; i < 3; i++){
				chave[i] = toupper(chave[i]);
			}

			//Pegar matricula
			string linhaP, vt_mat;
			getline(Lista, linhaP);
			matricula = linhaP.substr(41, 6);
			cout << matricula;

			Inserir(&(*indice), &(*no), anterior, chave, RRN, curso, indicelista);

			if((*indice) == 0){
				(*indice)++;
				Arquivos::LerListaOriginal(lista, &(*indice), Lista, indicelista, &((*no)->prox), *no);
			}
			else{
				(*indice)++;
				Arquivos::LerListaOriginal(lista, &(*indice), Lista, indicelista, &((*no)->prox), *no);
			}
		}
	} 
}

void Arquivos::ImprimeLista(){
   string linha;
   ifstream indicelista, Lista;
 
    indicelista.open("../res/indicelista.ind");
    Lista.open("../res/lista.txt");

   cout << "LISTA DE INDICES" << endl;
   cout << "Chave\t\t\t\tPRR do registro" << endl;
   while(!indicelista.eof()){
      getline(indicelista, linha);
      cout << linha << endl;
   }

   cout << "\nlista.txt" << endl;
   while(!Lista.eof()){
      getline(Lista, linha);
      cout << linha << endl;
   }            

   indicelista.close();
   Lista.close();   
}
