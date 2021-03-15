// Guilherme Moreira de Carvalho, 27/11/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Verificação de um Heap -  Programa que verifica se as chaves inteiras dos elementos de um vetor de tamanho N satisfazem a condição de um heap ou não.
// Referências: http://www2.dcc.ufmg.br/livros/algoritmos/cap3/codigo/c/3.1a3.2-lista-arranjo.c

#include <stdio.h>
#include <stdlib.h>

#define INICIOARRANJO   1
#define MAXTAM          1000
#define TRUE 1
#define FALSE 0

typedef int TipoChave;
typedef int TipoApontador;

typedef struct {
  TipoChave Chave;
} TipoItem;

typedef struct {
  TipoItem Item[MAXTAM];
  TipoApontador Primeiro, Ultimo;
} TipoLista;

/* ========================================================================== */

void FLVazia(TipoLista *Lista)
{ Lista -> Primeiro = INICIOARRANJO;
  Lista -> Ultimo = Lista -> Primeiro;
}  /* FLVazia */

int Vazia(TipoLista Lista)
{ return (Lista.Primeiro == Lista.Ultimo);
}  /* Vazia */

void Insere(TipoItem x, TipoLista *Lista)
{ if (Lista -> Ultimo > MAXTAM) printf("Lista esta cheia\n");
  else { Lista -> Item[Lista -> Ultimo - 1] = x;
         Lista -> Ultimo++;
       }
}  /* Insere */

void Retira(TipoApontador p, TipoLista *Lista, TipoItem *Item)
{ int Aux;

  if (Vazia(*Lista) || p >= Lista -> Ultimo) 
  { printf(" Erro   Posicao nao existe\n");
    return;
  }
  *Item = Lista -> Item[p - 1];
  Lista -> Ultimo--;
  for (Aux = p; Aux < Lista -> Ultimo; Aux++)
    Lista -> Item[Aux - 1] = Lista -> Item[Aux];
}  /* Retira */

void Imprime(TipoLista Lista)
{ int Aux;

  for (Aux = Lista.Primeiro - 1; Aux <= (Lista.Ultimo - 2); Aux++)
    printf("%d\n", Lista.Item[Aux].Chave);
}  /* Imprime */

int ver_heap(TipoLista lista)
{ int noInf = (int)(lista.Ultimo/2-1);  // nó inferior: maior nó pai possível

	for(int i=0; i<=noInf; i++){
		if(lista.Item[i].Chave < lista.Item[2*i+1].Chave || lista.Item[i].Chave < lista.Item[2*i+2].Chave)
			return FALSE;
	}
	return TRUE;
}  /* Verifica se uma lista está ordenada como um heap: nó pai (i) maior que nós filhos (2i+1 e 2i+2) */

/* ========================================================================== */

int main(int argc, char *argv[]){

	int n;
	TipoLista lista;
	TipoItem item;
	FLVazia(&lista);
	
	printf("Digite o tamanho do vetor: ");
	scanf(" %d", &n);	
	printf("Insira cada item do vetor a seguir:\n");
	
	for(int i=0; i<n; i++){
		scanf(" %d", &item.Chave);
		Insere(item, &lista);
	}
	
	if(ver_heap(lista))
		printf("\nResposta: O vetor e um heap.\n");
	else
		printf("\nResposta: O vetor nao e um heap.\n");
	
	return 0;
}
