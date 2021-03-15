// Guilherme Moreira de Carvalho, 11/09/2020 - Para Laborat�rio de Algoritmos e Estruturas de Dados I, 2020/2
//Programa que l� uma mensagem criptografada de um arquivo texto e imprime a mensagem descriptografada na sa�da padr�o.
//A t�cnica de criptografia deve ser c�digos em bloco (n = 2) e cifra de C�sar.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TAM;  // armazena o n�mero de bytes (tamanho) do arquivo

char* string(FILE* fp);
void printCript(FILE* fp, char* str);
void decript(char* cifra);

void decript(char* cifra){

	// Permuta os blocos (pares de 2 caracteres) trocando o primeiro com o �ltimo, o terceiro com a antepen�ltimo,
	// e assim por diante, mas deixando os outros como est�o
	char ch;
	for(int i = 0, j = TAM-2; i < j; i += 4, j -= 4){
		ch = cifra[i];
		cifra[i] = cifra[j-1];
		cifra[j-1] = ch;
		ch = cifra[i+1];
		cifra[i+1] = cifra[j];
		cifra[j] = ch;
	}

	// Reflete cada bloco
	for(int i = 0; i < TAM-2; i += 2){
		ch = cifra[i];
		cifra[i] = cifra[i+1];
		cifra[i+1] = ch;
	}

	// Substitui cada letra por outra, baseado na cifra de C�sar com uma rota��o � direita de tr�s posi��es
	// e os s�mbolos #s por espa�os
	int c;
	for(int i = 0; i < TAM-1; i++){
		if(cifra[i] == '#')
			cifra[i] = ' ';
		else{
			c = (int)cifra[i];
			if((c < 68) || (c > 90 && c < 100)) // A-Z [65:90], a-z [97:122]
                	    c += 26;                        // o caractere cifrado deve estar ao menos 3 posi��es dos limites
			cifra[i] = (char)c-3;
		}
	}
}

void printCript(FILE* fp, char* str){
	 
	//copia o arquivo
	for(int i = 0; i < TAM-1; i++){
		str[i] = fgetc(fp);
	}

	// imprime a mensagem criptografada
	putchar('\n');
	puts("--------------------");
	puts("Mensagem codificada:");
	puts("--------------------");

	for(int i = 0; i < TAM-1; i++){
		printf("%c", str[i]);
	}
}

char* string(FILE* fp){

	fseek(fp, 0L, SEEK_END);
	TAM = ftell(fp);
	char* str = (char*)malloc(sizeof(char)*(TAM-1));  // string com o tamanho do arquivo subtra�do o �ltimo byte (caractere '\n') [0:tam-2]
                                                   	    // armazenar� as mensagens criptografada e, ap�s as opera��es, descriptografada
	return str;
}

int main(){

	FILE* fp;
	char arquivo[20];
	printf("Digite o nome do arquivo: ");
	scanf("%s*c", arquivo);
	fp = fopen(arquivo, "r");
	if(fp == NULL){
		printf("Nao foi possivel abrir o arquivo!\n");
		return 1;
	}

	char* cifra = string(fp);
	rewind(fp);
	printCript(fp, cifra);
	putchar('\n');
	decript(cifra);

	// imprime a mensagem descriptografada
	putchar('\n');
	puts("----------------------");
	puts("Mensagem decodificada:");
	puts("----------------------");
	for(int i = 0; i < TAM-1; i++){
		printf("%c", cifra[i]);
	}
	putchar('\n');
	
	fclose(fp);
	free(cifra);
	return 0;
}
