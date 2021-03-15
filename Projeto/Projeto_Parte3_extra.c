// Guilherme Moreira de Carvalho, 19/11/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Projeto (Parte III, Extra) - Busca por faixa de pedestres na pista
// Referências: http://www2.dcc.ufmg.br/livros/algoritmos/cap3/codigo/c/3.1a3.2-lista-arranjo.c

#include <stdio.h>
#include <stdlib.h>

#define INICIOARRANJO   1
#define MAXTAM          1000
#define TRUE 1
#define FALSE 0

typedef int TipoChave;
typedef int TipoSegmento;
typedef int NumElementos;
typedef int TipoApontador;

typedef struct {
  TipoChave Chave;
  TipoSegmento Tipo;
  NumElementos n;
} TipoItem;

typedef struct {
  TipoItem Item[MAXTAM];
  TipoApontador Primeiro, Ultimo;
} TipoLista;

int faixa_pedestre[13] = {0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0};

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


// verifica se o padrão está presente na lista a partir de um índice
int ver_sequencia(TipoLista Lista, TipoChave Chave, int max){
	if(Chave + 12 > max)
		return FALSE;
	for(int i=0; i<13; i++){
		if(Lista.Item[Chave+i].Tipo != faixa_pedestre[i])
			return FALSE;
	}
	return TRUE;
}

/* ========================================================================== */

int main(int argc, char *argv[])
{ //abertura do arquivo
	FILE* fp;
	char arq[10];
	printf("Digite o nome do arquivo: ");
	scanf(" %s", arq);
	fp = fopen(arq, "r");
	if(fp == NULL){
		printf("Nao foi possivel abrir o arquivo!\n");
		return 1;
	}
	rewind(fp);
	
	//lê quantas linhas a imagem tem
	int L;
	fscanf(fp, " %d", &L);
	fgetc(fp);
	
	int y;
	int N;         //quantos elementos uma linha tem
	int x;		   //para verificar mudança de segmento de cor na linha
	int cont = 0;  //conta a recorrência em sequência do elemento
	int k = 0;     //chaves sequenciais de cada item inserido nas listas
	TipoLista sequencia[L];
	TipoItem item;
	
	for(int j=0; j<L; j++){
		FLVazia(&sequencia[j]);
		//constrói os segmentos
		fscanf(fp, " %d", &N);
		fgetc(fp);
		fscanf(fp, " %d", &y);
		cont++;
		for(int i=1; i<N; i++){
			fscanf(fp, " %d", &x);
			if(x == y)  //se o segmento não mudou, soma 1 à quantidade de ocorrências em sequência
				cont++;
			else{
				item.Tipo = y;
				y = x;    //muda o comparador
				item.Chave = k;
				item.n = cont;
				Insere(item, &sequencia[j]);
				cont = 1;   //o primeiro elemento do segmento já foi encontrado durante a mudança
				k++;
			}
		}
		//para o último segmento de cada linha - a iteração anterior trata casos de mudança de segmento
		item.Tipo = y;
		item.Chave = k;
		item.n = cont;
		Insere(item, &sequencia[j]);
		cont = 0;
		k = 0;
		fgetc(fp);
	}
	
	fclose(fp);  //fecha o arquivo

	for(int i=0; i<L; i++){
		for(int j=0; j<sequencia[i].Ultimo-11; j++){
			if(ver_sequencia(sequencia[i], j, sequencia[i].Ultimo)){
				printf("Resultado: Pista com faixa de pedestres\n");
				return 0;
			}
		}
	}
	
	printf("Resultado: Pista sem faixa de pedestres\n");
	return 0;
}
