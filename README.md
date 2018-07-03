# Organização de Arquivos - Trabalho 2
#### Manipulação de arquivo usando a estruturs Árvore B

## Alunas
	Hevelyn Sthefany Lima de Carvalho 17/0059031
	Ayllah Ahmad Lopes 17/0056465


## Sobre o Programa

	O código foi feito baseado nos algoritmos proposto por Michael J. Folk e Bill Zoellick no livro File Structures. A lógica de promoção da organização da árvore considera a quantidade de chaves mínimas que cada página deve ter, sendo que na inserção de uma chave, o progama promove a chave intermediária do conjunto de chaves a serem redistribuídos nas páginas (antiga e a criada).

	Para funcionamento correto, o programa aceita trabalhar com posições relativas de registros (RRN) de até 7 dígitos.

	Além disso, o arquivo a ser trabalhado deve ter o nome e formato lista.txt incluído no diretório res do projeto.

	
## Compilação e Execução

	Caminhe para o diretório src contido no diretório do projeto. Digite no terminal os seguintes comandos:
	$ make
	$ ./main

## Arquivos Gerados
	
	Será gerado e manipulado o arquivo de índices primários "indicelista.bt" também contido no diretório res do projeto.
	Tal arquivo é organizado de forma em que a primeira linha contém a posição relativa da pagina raiz no arquivo de Indices. A cada duas linhas é representada uma pagina. A primeira contém as chaves e a segunda as posições relativas (rrn) das páginas filhas (-1 quando não há página filha). As chaves são seguidas dos rrn do registro correspondente no arquivo original lista.txt, sendo separados pelo caractere "|".

	Novas páginas são acrescentadas sempre ao fim do arquivo e o rrn da pagina raiz sempre atualizado.


## GitHub

	Segue o link do repositório do projeto no github:
	https://github.com/HSuper-8/BTree_OA