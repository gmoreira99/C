// Guilherme Moreira de Carvalho, 22/09/2020 - Para Laboratório de Algoritmos e Estruturas de Dados I, 2020/2
// Problema da Mochila

#include <stdio.h>
#include <math.h>

void binario(int* a, int n);

//tipifica itens com peso e valor
typedef struct{
	int w, v;
} ITEM;

//Realiza uma contagem binária para testar todas as combinações de items
void binario(int* a, int n){

	int carry = 1;
	int soma;
	
	for(int i=n-1; i >= 0; i--){
		soma = a[i] + carry;
		switch(soma){
			case 2:
				a[i] = 0;
				carry = 1;
				break;
        		case 1:
            			a[i] = 1;
            			carry = 0;
            			break;
        		default:
            			a[i] = 0;
            			carry = 0;
        	}
	}
}

int main(){

	//abertura do arquivo
	FILE *fp;
	char arq[10];
	printf("Digite o nome do arquivo: ");
	scanf(" %s", arq);
	fp = fopen(arq, "r");
	if(fp == NULL){
		printf("Não foi possivel abrir o arquivo!\n");
		return 1;
	}
	rewind(fp);
	
	//lê do arquivo a capacidade da mochila
	//e o total de itens disponíveis
	int W;
	int n_items;
	fscanf(fp, " %d", &W);
	fgetc(fp);
	fscanf(fp, " %d", &n_items);
	fgetc(fp);
	//teste
	//printf("W = %d\nn_items = %d\n", W, n_items);
	
	//cria um vetor de itens do tamanho da quantidade de items disponíveis
	//lê os atributos de cada item e o armazena em uma posição correspondente
	ITEM items[n_items];
	for(int i=0; i<n_items; i++){
		fscanf(fp, " %d%d", &items[i].w, &items[i].v);
		fgetc(fp);
	}
	
	int combinacao[n_items];
	//Calcula o valor máximo que um conjunto de itens com peso menor ou igual
	//à capacidade da mochila pode ter
	int v_max = 0;
	int v_max_aux, w_aux;
	
	for(int i=0; i<pow(2, n_items); i++){
		v_max_aux = 0;
		w_aux = 0;
		binario(combinacao, n_items);
		for(int j=n_items-1; j>=0; j--){
			if(combinacao[j] == 1){
				w_aux += items[j].w;
				v_max_aux += items[j].v;
			}
		}
		if(w_aux > W)
			continue;
		if(v_max_aux > v_max)
			v_max = v_max_aux;
	}
	
	v_max_aux = 0;
	w_aux = 0;
	
	//verifica quais itens o vencedor escolheu
	int ver;
	for(int i=0; i<n_items; i++){
		fscanf(fp, " %d", &ver);
		fgetc(fp);
		if(ver){
			v_max_aux += items[i].v;
			w_aux += items[i].w;
		}
	}
	//teste
	//printf("Valor: %d\tValor Maximo: %d\nPeso: %d\tPeso Maximo: %d\n", v_max_aux, v_max, w_aux, W);
	
	//determina o tipo da solução
	if(w_aux <= W){  //soluções viáveis
		if(v_max_aux == v_max)
			printf("Solucao otima.\n");
		else
			printf("Solucao viavel mas nao otima.\n");
	}
	else  //soluções inviáveis
		printf("Solucao inviavel.\n");
	
	fclose(fp);
	return 0;
}
