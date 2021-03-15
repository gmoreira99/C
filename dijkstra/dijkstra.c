// Guilherme Moreira de Carvalho, 30/08/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Programa que, dadas N cidades e as distâncias entre elas, calcula o total de quilômetros do menor percurso entre duas dessas cidades. As distâncias devem ser números inteiros, e os nomes das cidades, strings com até 20 caracteres (sem espaços em branco).
// Referências: http://www2.dcc.ufmg.br/livros/algoritmos/cap7/codigo/c/7.22-dijkstramatriz.c/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include<string.h>

#define MAXNUMVERTICES  100
#define MAXNUMARESTAS   4500
#define FALSE           0
#define TRUE            1
#define INFINITO       INT_MAX

typedef int TipoValorVertice;

typedef int TipoPeso;

typedef struct TipoGrafo {
  TipoPeso Mat[MAXNUMVERTICES + 1][MAXNUMVERTICES + 1];
  int  NumVertices;
  int  NumArestas;
} TipoGrafo;

typedef TipoValorVertice TipoApontador;
typedef int TipoIndice;
typedef struct TipoItem {
  TipoPeso Chave;
} TipoItem;

typedef TipoItem TipoVetor[MAXNUMVERTICES + 1];

TipoApontador Aux;
int  i, NArestas;
short FimListaAdj;
TipoValorVertice V1, V2, Adj;
TipoPeso Peso;
TipoGrafo Grafo;
TipoIndice n;   /*Tamanho do heap*/

TipoValorVertice NVertices = 0;
TipoValorVertice Raiz;     /*índice da cidade origem na matriz de mapeamento*/

char cidades[10][20];      /*matriz de mapeamento das cidades*/
TipoValorVertice Destino;  /*índice da cidade destino na matriz de mapeamento*/

void FGVazio(TipoGrafo *Grafo)
{ int  i, j;
  for (i = 0; i <= Grafo->NumVertices; i++)
    for (j = 0; j <= Grafo->NumVertices; j++)
      Grafo->Mat[i][j] = 0;
}

void InsereAresta(TipoValorVertice *V1, TipoValorVertice *V2, TipoPeso *Peso,
          TipoGrafo *Grafo)
{ Grafo->Mat[*V1][*V2] = *Peso;
}

short  ExisteAresta(TipoValorVertice Vertice1, TipoValorVertice Vertice2,
            TipoGrafo *Grafo)
{ return (Grafo->Mat[Vertice1][Vertice2] > 0);
}

/*-- Operadores para obter a lista de adjacentes --*/

short ListaAdjVazia(TipoValorVertice *Vertice, TipoGrafo *Grafo)
{ TipoApontador Aux = 0;
  short ListaVazia = TRUE;
  while (Aux < Grafo->NumVertices && ListaVazia)
    { if (Grafo->Mat[*Vertice][Aux] > 0)
      ListaVazia = FALSE;
      else
      Aux++;
    }
  return (ListaVazia == TRUE);
}

TipoApontador PrimeiroListaAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo)
{ TipoValorVertice Result;
  TipoApontador Aux = 0;
  short Listavazia = TRUE;
  while (Aux < Grafo->NumVertices && Listavazia)
    { if (Grafo->Mat[*Vertice][Aux] > 0)
      { Result = Aux; Listavazia = FALSE;
      }
      else  Aux++;
    }
  if (Aux == Grafo->NumVertices)
  printf("Erro: Lista adjacencia vazia (PrimeiroListaAdj)\n");
  return Result;
}

void ProxAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo,
         TipoValorVertice *Adj, TipoPeso *Peso, TipoApontador *Prox,
         short *FimListaAdj)
{ /* --Retorna Adj apontado por Prox--*/
  *Adj = *Prox;
  *Peso = Grafo->Mat[*Vertice][*Prox];
  (*Prox)++;
  while (*Prox < Grafo->NumVertices && Grafo->Mat[*Vertice][*Prox] == 0)
    (*Prox)++;
  if (*Prox == Grafo->NumVertices)  *FimListaAdj = TRUE;
}

void ImprimeGrafo(TipoGrafo *Grafo)
{ int i, j;
  printf("   ");
  for (i = 0; i < Grafo->NumVertices; i++) printf("%3d", i); putchar('\n');
  for (i = 0; i < Grafo->NumVertices; i++)
    { printf("%3d", i);
      for (j = 0; j < Grafo->NumVertices; j++)
        printf("%3d", Grafo->Mat[i][j]);
      putchar('\n');
    }
}

void RefazInd(TipoIndice Esq, TipoIndice Dir, TipoItem *A, TipoPeso *P,
            TipoValorVertice *Pos)
{ TipoIndice i = Esq;  int j;  TipoItem x;
  j = i * 2;  x = A[i];
  while (j <= Dir)
    { if (j < Dir)
      { if (P[A[j].Chave] > P[A[j+1].Chave]) j++;
      }
      if (P[x.Chave] <= P[A[j].Chave])  goto L999;
      A[i] = A[j]; Pos[A[j].Chave] = i;
      i = j;   j = i * 2;
    }
  L999:  A[i] = x;  Pos[x.Chave] = i;
}

void Constroi(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)
{ TipoIndice Esq;
  Esq = n / 2 + 1;
  while (Esq > 1)
    { Esq--;
      RefazInd(Esq, n, A, P, Pos);
    }
}

TipoItem RetiraMinInd(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem Result;
  if (n < 1)
  { printf("Erro: heap vazio\n");
    return Result;
  }
  Result = A[1];
  A[1] = A[n]; Pos[A[n].Chave] = 1;
  n--;
  RefazInd(1, n, A, P, Pos);
  return Result;
}

void DiminuiChaveInd(TipoIndice i, TipoPeso ChaveNova, TipoItem *A,
             TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem x;
  if (ChaveNova > P[A[i].Chave])
  { printf("Erro: ChaveNova maior que a chave atual\n");
    return;
  }
  P[A[i].Chave] = ChaveNova;
  while (i > 1 && P[A[i / 2].Chave] > P[A[i].Chave])
    { x = A[i / 2];
      A[i / 2] = A[i];
      Pos[A[i].Chave] = i / 2;
      A[i] = x;
      Pos[x.Chave] = i;
      i /= 2;
    }
}

int Dijkstra(TipoGrafo *Grafo, TipoValorVertice *Raiz, TipoValorVertice *Destino)
{ TipoPeso P[MAXNUMVERTICES + 1];
  TipoValorVertice Pos[MAXNUMVERTICES + 1];
  long Antecessor[MAXNUMVERTICES + 1];
  //short Itensheap[MAXNUMVERTICES + 1];
  TipoVetor A;
  TipoValorVertice u, v;
  TipoItem temp;

  int distancia = 0; 		 /*armazena a distância total da origem até a última aparição (menor distância) da cidade destino na lista de adjacência*/
  char percurso[NVertices][20];  /*armazena o menor percurso ao contrário*/
  int percurso_aux; 		 /*para construção, cidade por cidade, do percurso*/

  for (u = 0; u <= Grafo->NumVertices; u++)
  { /*Constroi o heap com todos os valores igual a INFINITO*/
    Antecessor[u] = -1; P[u] = INFINITO;
    A[u+1].Chave = u;   /*Heap a ser construido*/
    /*Itensheap[u] = TRUE;*/  Pos[u] = u + 1;
  }
  n = Grafo->NumVertices;
  P[*(Raiz)] = 0;
  Constroi(A, P, Pos);
  while (n >= 1)
  { /*enquanto heap nao vazio*/
    temp = RetiraMinInd(A, P, Pos);
    u = temp.Chave; /*Itensheap[u] = FALSE;*/
    if (!ListaAdjVazia(&u, Grafo))
    { Aux = PrimeiroListaAdj(&u, Grafo); FimListaAdj = FALSE;
      while (!FimListaAdj)
      { ProxAdj(&u, Grafo, &v, &Peso, &Aux, &FimListaAdj);
        if (P[v] > (P[u] + Peso))
        { P[v] = P[u] + Peso; Antecessor[v] = u;
          DiminuiChaveInd(Pos[v], P[v], A, P, Pos);

          /*printf("Caminho: v[%d] v[%ld] d[%d]\n",
	         v, Antecessor[v], P[v]);*/
          //scanf("%*[^\n]");
          //getchar();

          if(v==*Destino){
	    percurso_aux = v;  /*armazena a cidade de interesse*/
            distancia = P[percurso_aux];
	  }
  
        }
      }
    }
  }

  strcpy(percurso[0], cidades[percurso_aux]);  /*a primeira cidade a ser registrada no percurso é a cidade destino*/
  int i = 1;
  while(percurso_aux!=*Raiz){  /*até voltar a origem*/
   strcpy(percurso[i], cidades[Antecessor[percurso_aux]]);
   percurso_aux = Antecessor[percurso_aux];
   ++i;
  }

  printf("Menor percurso:");
  for(--i; i>=0; i--)  /*imprime o percurso de trás para frente (da origem ao destino)*/
    printf(" %s", percurso[i]);
  printf("\n");

  return distancia;

}

/*Realiza o mapeamento das cidades*/
void adcCidade(char *cidade){
    int adc = TRUE;
    //controla a adição de uma nova cidade na matriz

    for(int i=0; i<NVertices; i++){
        if(!strcmp(cidades[i], cidade)){
            adc = FALSE;
            break;
        }
    }

    if(adc){
        strcpy(cidades[NVertices], cidade);
	NVertices++;
    }
}


int main(int argc, char *argv[])
{  /*-- Programa principal --*/

  //abertura do arquivo
  FILE *fp;
  char arq[15];
  printf("Digite o nome do arquivo de entrada: ");
  scanf("%s*c", arq);
  fp = fopen(arq, "r");
  if(fp == NULL){
    printf("Nao foi possivel abrir o arquivo!");
    return 1;
  }
  rewind(fp);

  /* -- NumVertices: definido antes da leitura das arestas --*/
  /* -- NumArestas: inicializado com zero e incrementado a --*/
  /* -- cada chamada de InsereAresta                    --*/

  fscanf(fp, "%d*c", &NArestas); /*lê a primeira linha do arquivo (n. de arestas)*/
  fgetc(fp);
  Grafo.NumArestas = 0;

  FGVazio(&Grafo);

  char cidadeX[20], cidadeY[20];
  //lê as linhas do arquivo e faz o mapeamento das cidades
  for(int i=0; i<NArestas; i++){
    fscanf(fp, "%s%s%d*c", cidadeX, cidadeY, &Peso);
    fgetc(fp);
    adcCidade(cidadeX);
    adcCidade(cidadeY);
  }
  Grafo.NumVertices = NVertices;

  rewind(fp);
  fscanf(fp, "%d*c", &NArestas); /*reposiciona o leitor do arquivo*/
  fgetc(fp);

  //lê as linhas do arquivo e constrói o grafo
  for(int i=0; i<NArestas; i++){
    fscanf(fp, "%s%s%d*c", cidadeX, cidadeY, &Peso);
    fgetc(fp);
    for(int j=0; j<NVertices; j++){
      if(!strcmp(cidades[j], cidadeX)){
        V1 = j;
      }else
      if(!strcmp(cidades[j], cidadeY)){
        V2 = j;
      }
    }
    /*1 chamada : G direcionado*/
    /*2 chamadas: G nao-direcionado*/
    InsereAresta(&V1, &V2, &Peso, &Grafo);
    InsereAresta(&V2, &V1, &Peso, &Grafo);
    Grafo.NumArestas++;
  }

  /*lê a última linha do arquivo: cidades origem e destino*/
  fscanf(fp, "%s%s*c", cidadeX, cidadeY);
  for(int i=0; i<NVertices; i++){
    if(!strcmp(cidadeX, cidades[i]))
        Raiz = i;
    else
    if(!strcmp(cidadeY, cidades[i]))
        Destino = i;
  }

  /*PARA TESTES*/
  //ImprimeGrafo(&Grafo);
  /*printf("n. vertices: %d\nn. arestas: %d\n", NVertices, NArestas);
  for(int i=0; i<NVertices; i++)
    printf("%s ", cidades[i]);
  printf("\n%s: %d\n%s: %d\n", cidadeX, Raiz, cidadeY, Destino);*/

  int distancia = Dijkstra(&Grafo, &Raiz, &Destino);
  printf("Distancia total: %d Km", distancia);
  
  fclose(fp);
  return 0;

}