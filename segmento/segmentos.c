// Guilherme Moreira de Carvalho, 2/10/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Programa que, dado um vetor, produz uma matriz com 2 linhas e K colunas, ALOCADAS DINAMICAMENTE, de forma que a primeira linha contém informações sobre o tipo dos segmentos e a segunda linha, o número de elementos do respectivo segmento, na ordem em que aparecem no vetor.
// O vetor é fornecido por um arquivo texto com o seguinte formato: na primeira linha o valor de N e na linha seguinte os N valores dos elementos do vetor separados por um espaço em branco.

#include <stdio.h>
#include <stdlib.h>

int ver(int* v, int n, int x);

//verifica se um elemento x está contido no array v de tamanho n
int ver(int*v, int n, int x){
	for(int i=0; i<n; i++){
		if(v[i] == x)
			return 1;
	}
	return 0;
}

int main(){

	//abertura do arquivo
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
		if(!ver(v, n_aux, x)){  //mapeia um novo elemento
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
	int k = 0;      //quantidade de colunas da matriz resultado
	int j = 0;
	
	int** matriz = (int**) malloc(2*sizeof(int*));  //memória alocada para a matriz
	for(int i=0; i<2; i++){
		matriz[i] = (int*) malloc(sizeof(int));
	}
	
	//constrói a matriz resultado
	for(int i=0; i<N; i++){
		fscanf(fp, " %d", &x);
		if(x == y)  //se o segmento não mudou, soma 1 à quantidade de ocorrências em sequência
			cont++;
		else{
			for(; j<n_aux; j++)  //identifica o tipo do segmento
				if(y == v[j])
					break;
			y = x;  //muda o comparador
			k++;
			matriz[0] = (int*) realloc(matriz[0], k*sizeof(int));  //cria uma nova coluna com o tipo e tamanho do segmento
			matriz[1] = (int*) realloc(matriz[1], k*sizeof(int));
			matriz[0][k-1] = j+1;
			matriz[1][k-1] = cont;
			cont = 1;  //o primeiro elemento do segmento já foi encontrado durante a mudança
			j = 0;
		}
	}
	for(; j<n_aux; j++)  //identifica o tipo do último segmento - a iteração trata casos de mudança de segmento
		if(y == v[j])
			break;
	k++;
	matriz[0] = (int*) realloc(matriz[0], k*sizeof(int));  //cria uma nova coluna com o tipo e tamanho do segmento
	matriz[1] = (int*) realloc(matriz[1], k*sizeof(int));
	matriz[0][k-1] = j+1;
	matriz[1][k-1] = cont;
	
	printf("Matriz:\n");  //imprime a matriz
	for(int i = 0; i<2; i++){
		for(int j=0; j<k; j++){
			printf("%d ", matriz[i][j]);
		}
		putchar('\n');
	}
	
	//libera a memória alocada e fecha o arquivo
	free(v);
	free(matriz);
	fclose(fp);
	return 0;
}
