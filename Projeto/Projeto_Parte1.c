// Guilherme Moreira de Carvalho, 24/10/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Projeto (Parte I) - Busca por padrão em lista
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

int padrao[5] = {1, 3, 2, 3, 1};

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


// verifica se um elemento x está contido no array v de tamanho n
int ver_item(int*v, int n, int x){
	for(int i=0; i<n; i++){
		if(v[i] == x)
			return TRUE;
	}
	return FALSE;
}

// verifica se o padrão está presente em alguma parte da lista
int ver_sequencia(TipoLista sequencia, TipoChave Chave, int max){
	if(Chave + 4 > max)
		return FALSE;
	for(int i=0; i<5; i++, Chave++){
		if(sequencia.Item[Chave].Tipo != padrao[i])
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
	
	//lê quantos elementos o vetor tem
	int N;
	fscanf(fp, " %d", &N);
	fgetc(fp);
	
	//mapeia os elementos do vetor
	int x;
	int *v = (int*) malloc(sizeof(int));  //memória alocada para o vetor de mapeamento
	int n_aux = 0;  		       //quantidade de elementos distintos do vetor
	for(int i=0; i<N; i++){
		fscanf(fp, " %d", &x);
		if(!ver_item(v, n_aux, x)){  //mapeia um novo elemento
			n_aux++;
			v = (int*) realloc(v, n_aux*sizeof(int));
			v[n_aux-1] = x;
		}
	}
	rewind(fp);  //reposiciona o leitor do arquivo
	fscanf(fp, " %d", &N);
	fgetc(fp);
	
	int y = v[0];   //para verificar a mudança de segmento do vetor
	int cont = 0;   //conta a recorrência em sequência do elemento
	int k = 0;      //chaves sequenciais de cada elemento inserido na lista
	int j = 0;
	TipoLista sequencia;
	TipoItem item;
	FLVazia(&sequencia);
	
	//constrói a lista resultado
	for(int i=0; i<N; i++){
		fscanf(fp, " %d", &x);
		if(x == y)  //se o segmento não mudou, soma 1 à quantidade de ocorrências em sequência
			cont++;
		else{
			for(; j<n_aux; j++)  //identifica o tipo do segmento
				if(y == v[j])
					break;
			y = x;  //muda o comparador
			item.Chave = k;
			item.Tipo = j+1;
			item.n = cont;
			Insere(item, &sequencia);
			cont = 1;   //o primeiro elemento do segmento já foi encontrado durante a mudança
			k++;
			j = 0;
		}
	}
	for(; j<n_aux; j++)  //identifica o tipo do último segmento - a iteração anterior trata casos de mudança de segmento
		if(y == v[j])
			break;
	item.Chave = k;
	item.Tipo = j+1;
	item.n = cont;
	Insere(item, &sequencia);

	//libera a memória alocada e fecha o arquivo
	free(v);
	fclose(fp);

	for(int i=0; i<k-3; i++){
		if(ver_sequencia(sequencia, i, k)){
			printf("Resultado: Padrao encontrado.\n");
			return 0;
		}
	}
	printf("Resultado: Padrao nao encontrado.\n");
	return 0;
}
