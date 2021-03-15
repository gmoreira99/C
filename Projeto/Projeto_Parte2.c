// Guilherme Moreira de Carvalho, 30/10/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Projeto (Parte II) - Cálculo do ponto médio da pista
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
typedef int PontoMedio;
typedef int TipoApontador;

typedef struct {
  TipoChave Chave;
  TipoSegmento Tipo;
  NumElementos n;
  PontoMedio m;
} TipoItem;

typedef struct {
  TipoItem Item[MAXTAM];
  TipoApontador Primeiro, Ultimo;
} TipoLista;

int padrao[5] = {0, 255, 128, 255, 0};

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
	if(Chave + 4 > max)
		return FALSE;
	for(int i=0; i<5; i++){
		if(Lista.Item[Chave+i].Tipo != padrao[i])
			return FALSE;
	}
	return TRUE;
}

// retorna o ponto médio do terceiro item a partir de um índice de uma lista
int p_medio(TipoLista Lista, TipoChave Chave){
	return Lista.Item[Chave+2].m;
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
	
	//lê quantos elementos o vetor tem
	int N;
	fscanf(fp, " %d", &N);
	fgetc(fp);
	
	int y;
	int x;  	   //para verificar a mudança de segmento do vetor
	int cont = 0;  //conta a recorrência em sequência do elemento
	int k = 0;     //chaves sequenciais de cada elemento inserido na lista
	int q, p = 0;  //registram, respectivamente, o fim e o início de um segmento
	TipoLista sequencia;
	TipoItem item;
	FLVazia(&sequencia);
	
	fscanf(fp, " %d", &y);
	cont++;
	
	//constrói a lista resultado
	for(int i=1; i<N; i++){
		fscanf(fp, " %d", &x);
		if(x == y)  //se o segmento não mudou, soma 1 à quantidade de ocorrências em sequência
			cont++;
		else{	
			item.Tipo = y;
			y = x;    //muda o comparador
			q = i-1;  //o segmento terminou na iteração anterior - antes da mudança de segmento
			item.Chave = k;
			item.n = cont;
			item.m = (p+q)/2;
			Insere(item, &sequencia);
			cont = 1;   //o primeiro elemento do segmento já foi encontrado durante a mudança
			k++;
			p = i;      //o novo segmento começa na iteração atual - após a mudança de segmento
		}
	}
	//para o último segmento - a iteração anterior trata casos de mudança de segmento
	q = N-1;
	item.Chave = k;
	item.Tipo = y;
	item.n = cont;
	item.m = (p+q)/2;
	Insere(item, &sequencia);

	fclose(fp);  //fecha o arquivo

	for(int i=0; i<k-3; i++){
		if(ver_sequencia(sequencia, i, k)){
			printf("Resultado: Padrao encontrado.\n");
			printf("Ponto Medio: %d\n", p_medio(sequencia, i));
			return 0;
		}
	}
	printf("Resultado: Padrao nao encontrado.\n");
	return 0;
}
