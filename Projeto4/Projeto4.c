#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct VOO 
{
	char idVoo[7];
	char status;
	int  prioridade;
	struct VOO *prox;
} Voo;

typedef struct FILA
{
	Voo *inicio;
	Voo *fim;
} Fila;

Fila *criaFila();
// Fila *insereVoo(Voo *);
Voo *criaVoo(char *, char , int);

int main(int argc, char const *argv[])
{
	char *idVoo[64] = 
	{
		"VG3001", "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008", 
		"AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012", "TT4544", 
		"TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002", "AZ1007", "GL7604", 
		"AZ1006", "TG1503", "AZ1003", "JJ4403", "AZ1001", "LN7003", "AZ1004", "TG1504", 
		"AZ1005", "TG1502", "GL7601", "TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", 
		"VV3390", "VV3392",	"GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", 
		"AZ1065",	"LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
		"TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609", "KL5610", "KL5611"
	};

	int numDecola = 0, numAproxima = 0, numTotalVoos = 0, vooAleatorio;
	int  *selecionaIdVoo = (int *) calloc(64,sizeof(int));
	Fila *filaAproxima, *filaDecola;

	filaAproxima = criaFila();
	filaDecola   = criaFila();

/********************************** Processo para Aleatorizar ********************************/
	srand(time(NULL));
	numAproxima = 10 + (rand() % 22);
	numDecola = 10 + (rand() % 22);
	numTotalVoos = numAproxima + numDecola;

	for (int i = 0; i < numDecola; i++)
	{	
		srand(time(NULL)+1);
		vooAleatorio =  rand() % 64;

		if (*(selecionaIdVoo + vooAleatorio) == 1)
		{
			i--;
		}
		else if (*(selecionaIdVoo + vooAleatorio) == 0)
		{
			*(selecionaIdVoo + vooAleatorio) = 1;
			filaDecola = insereVoo(criaVoo(idVoo[vooAleatorio],'D', 12));
		}
	}


	free(selecionaIdVoo);
/*********************************************************************************************/

	while(filaDecola->inicio->prox != NULL)
	{
		printf("%s %d %d",filaDecola->inicio->idVoo, filaDecola->inicio->status, filaDecola->inicio->prioridade);
		filaDecola->inicio->prox = filaDecola->inicio->prox;
	}

	return 0;
}

Fila *criaFila()
{
	Fila *fila = (Fila *)malloc(sizeof(Fila));
	
	if (fila != NULL)
		fila->inicio = fila->fim = NULL;
	else 
		exit(1);

	return fila;
}

Voo *criaVoo(char *idVoo, char status, int prioridade)
{
	Voo *novoVoo = (Voo *) malloc(sizeof(Voo));
	strcpy(novoVoo->idVoo, idVoo);
	novoVoo->status = status;
	novoVoo->prioridade = prioridade;
	novoVoo->prox = NULL;

	return novoVoo;
}