#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#define m               2
#define mm              (m * 2)
#define FALSE           0
#define TRUE            1

typedef struct REGISTRO {
  int chave;
  /*outros componentes*/
} REGISTRO;

typedef struct PAGINA* APONTADOR;

typedef struct PAGINA {
  short n;
  REGISTRO r[mm];
  APONTADOR p[mm + 1];
} PAGINA;

REGISTRO* pesquisar(int chave, APONTADOR a)
{ 
  long i = 0;
  if (a == NULL) return (NULL);
  while (i < a->n && chave > a->r[i].chave) i++;
  if (chave == a->r[i].chave) return &a->r[i];
  if (chave < a->r[i].chave)
    return (pesquisar(chave, a->p[i]));
  else
    return (pesquisar(chave, a->p[i+1]));
}

void insere_na_pagina(APONTADOR ap, REGISTRO reg, APONTADOR ap_dir)
{ 
  int k;
  for (k = ap->n; (k > 0) && (reg.chave < ap->r[k-1].chave); k--)
  {
    ap->r[k] = ap->r[k-1];
    ap->p[k+1] = ap->p[k];    
  }
  ap->r[k] = reg;
  ap->p[k+1] = ap_dir;
  ap->n++;
} 

short insere_rec(REGISTRO reg, APONTADOR ap, REGISTRO *reg_retorno, APONTADOR *ap_retorno)
{ 
  short cresceu;
  long j, i = 0;
  APONTADOR ap_temp;

  if (ap == NULL)
  { *reg_retorno = reg; *ap_retorno = NULL;
    return TRUE;
  }
  
  while (i < ap->n && reg.chave > ap->r[i].chave) i++;

  if (reg.chave == ap->r[i].chave) 
  { printf(" Erro: REGISTRO ja esta presente\n");
    return FALSE;
  }

  cresceu = insere_rec(reg, ap->p[i], reg_retorno, ap_retorno);
  if (!cresceu) return (FALSE);

  if (ap->n < mm)   /* PAGINA tem espaco */
  { insere_na_pagina(ap, *reg_retorno, *ap_retorno);
    return (FALSE);
  }

  /* Overflow: PAGINA tem que ser dividida */
  ap_temp = (APONTADOR)malloc(sizeof(PAGINA));
  ap_temp->n = 0;  ap_temp->p[0] = NULL;
  if (i < m+1)
  { insere_na_pagina(ap_temp, ap->r[mm-1], ap->p[mm]);
    ap->n--;
    insere_na_pagina(ap, *reg_retorno, *ap_retorno);
  } 
  else
    insere_na_pagina(ap_temp, *reg_retorno, *ap_retorno);
  for (j = m + 2; j <= mm; j++)
    insere_na_pagina(ap_temp, ap->r[j-1], ap->p[j]);
  ap->n = m;  ap_temp->p[0] = ap->p[m+1];
  *reg_retorno = ap->r[m];
  *ap_retorno = ap_temp;
  return TRUE;
}

// Retorna ponteiro para a pagina raiz
APONTADOR insere(REGISTRO reg, APONTADOR ap_raiz)
{ 
  short cresceu;
  REGISTRO reg_retorno;
  APONTADOR ap_retorno, ap_temp;

  cresceu = insere_rec(reg, ap_raiz, ®_retorno, &ap_retorno);
  if (cresceu)  /* Arvore cresce na altura pela raiz */
  { 
    ap_temp = (PAGINA *)malloc(sizeof(PAGINA));
    ap_temp->n = 1;
    ap_temp->r[0] = reg_retorno;
    ap_temp->p[1] = ap_retorno;
    ap_temp->p[0] = ap_raiz;
    return ap_temp;
  }
  return ap_raiz;
}

void imprime(APONTADOR p, int nivel)
{ 
  long i;
  if (p == NULL) return;
  printf("Nivel %d : ", nivel);
  for (i = 0; i < p->n; i++)
    printf("%ld ",(long)p->r[i].chave);
  putchar('\n');
  for (i = 0; i <= p->n; i++)
    imprime(p->p[i], nivel+1);
} 

int main(int argc, char *argv[])
{ 
  REGISTRO x;
  APONTADOR D = 0;
//  clrscr();
  printf("inserir chave: ");
  scanf("%ld%*[^\n]", &x.chave);
  getchar();
  while (x.chave != 0) 
  { 
    D = insere(x, D);
    imprime(D,0);
    
    printf("pesquisar chave: ");
    scanf("%ld%*[^\n]", &x.chave);
    if (pesquisar(x.chave,D)) printf("chave encontrada!\n");
    else printf("chave NAO encontrada!\n");
  
    printf("inserir chave: ");
    scanf("%ld%*[^\n]", &x.chave);
    getchar();
  }
}