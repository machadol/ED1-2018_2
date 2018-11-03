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
}Voo;

typedef struct FILA 
{
  Voo *inicio;
  Voo *fim;
}Fila;

Fila *criaFila();
void insereFila(Fila *, char *, char , int);
void retiraFila(Fila *fila);
void imprimiFila(Voo *, char);
void imprimiEvento(char *, char, int, int, int, int);
void bubbleSort(Voo *);
void swap(Voo *, Voo *);


int main(int argc, char const *argv[])
{
	char *idVoo[64] = 
  {
    "VG3001", "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008", 
    "AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012", "TT4544", 
    "TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002", "AZ1007", "GL7604", 
    "AZ1006", "TG1503", "AZ1003", "JJ4405", "AZ1001", "LN7003", "AZ1004", "TG1504", 
    "AZ1005", "TG1502", "GL7601", "TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", 
    "VV3390", "VV3392", "GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", 
    "AZ1065", "LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
    "TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609", "KL5610", "KL5611"
  };

 	int numDecola, numAproxima, numTotalVoos, vooAleatorio, prioridade;
  int *selecionaIdVoo = (int *)calloc(64,sizeof(int));
  Fila *filaAproxima = criaFila();
  Fila *filaDecola   = criaFila();

/********************************** Processo para Aleatorizar ********************************/
  srand(time(NULL));
  numAproxima = 10 + (rand() % 22);
  numDecola   = 10 + (rand() % 22);
  numTotalVoos = numAproxima + numDecola;

  // Aleatorizando Decolagem
  srand(time(NULL)+1);
  int i = 1;
  do
  {   
    vooAleatorio =  rand() % 64;

    if (*(selecionaIdVoo + vooAleatorio) == 0)
    {
      *(selecionaIdVoo + vooAleatorio) = 1;
      insereFila(filaDecola, idVoo[vooAleatorio], 'D', 12);
      i++;
    }
  } while(i != numDecola);
  // Aleatorizando Aproximação
  srand(time(NULL)+1);
  int j = 1;
  do
  {   
    vooAleatorio =  rand() % 64;

    if (*(selecionaIdVoo + vooAleatorio) == 0)
    {
      *(selecionaIdVoo + vooAleatorio) = 1;
      prioridade = rand()%13;
      insereFila(filaAproxima, idVoo[vooAleatorio], 'A', prioridade);
      bubbleSort(filaAproxima->inicio);
      j++;
    }
  } while(j != numAproxima);


  free(selecionaIdVoo);

/*********************************************************************************************/

  printf("--------------------------------------------------------------------------------\n");
  printf("Aeroporto Internacional de Brasília\n");
  printf("Hora Inicial: 12:00\n");
  printf("Fila de Pedidos: [código do voo – P/D – prioridade]\n");
  printf("NVoos: %d\n", numTotalVoos);
  printf("Naproximações: %d\n", numAproxima);
  printf("NDecolagens: %d\n", numDecola);
  printf("--------------------------------------------------------------------------------\n");
  printf("\nFila de aproximações:\n");
  imprimiFila(filaAproxima->inicio, 'A');
  printf("\nFila de decolagens:\n");
  imprimiFila(filaDecola->inicio, 'D');
  printf("\n\nListagem de eventos:\n\n");

/*************************************** Controle de Voo *************************************/

  int hora = 12, minuto = 00;
  int pista1 = 0, pista2 = 0, pista3 = 0, unidadeTempo = 1;

  while(filaAproxima->inicio != NULL || filaDecola->inicio != NULL)
  {
  	int prioridadeZero = 0;
  	Voo *aux = filaAproxima->inicio;
  	while(aux != NULL)
  	{
  		if (aux->prioridade == 0)
  			prioridadeZero++;

  		aux = aux->prox;
  	}

  	if (prioridadeZero >= 3)
  		printf("ALERTA GERAL DE DESVIO DE AERONAVE \n");

		if (pista1 == 0)
		{
			if (filaAproxima->inicio != NULL)
			{
				imprimiEvento(filaAproxima->inicio->idVoo, 'A', hora, minuto, 1, filaAproxima->inicio->prioridade);
				retiraFila(filaAproxima);
				pista1 = 4; // 1 unidade de tempo aproximação + 3 unidade de tempo pouso
			}
			else if (filaDecola->inicio != NULL)
			{
				imprimiEvento(filaDecola->inicio->idVoo, 'D', hora, minuto, 1, filaDecola->inicio->prioridade);
				retiraFila(filaDecola);
				pista1 = 2; // 2 unidade de tempo decolagem
			}
		}

		if (pista2 == 0)
		{
			if (filaAproxima->inicio != NULL)
			{
				imprimiEvento(filaAproxima->inicio->idVoo, 'A', hora, minuto, 2, filaAproxima->inicio->prioridade);
				retiraFila(filaAproxima);
				pista2 = 4; // 1 unidade de tempo aproximação + 3 unidade de tempo pouso
			}
			else if (filaDecola->inicio != NULL)
			{
				imprimiEvento(filaDecola->inicio->idVoo, 'D', hora, minuto, 2, filaDecola->inicio->prioridade);
				retiraFila(filaDecola);
				pista2 = 2; // 2 unidade de tempo decolagem
			}
		}

		if (prioridadeZero >= 3)
		{
			if (pista3 == 0)
			{
				imprimiEvento(filaAproxima->inicio->idVoo, 'A', hora, minuto, 3, filaAproxima->inicio->prioridade);
				retiraFila(filaAproxima);
				pista3 = 4; // 1 unidade de tempo aproximação + 3 unidade de tempo pouso
			}
		}
		
		if (pista3 == 0)
		{
			if (filaDecola->inicio != NULL)
			{
				imprimiEvento(filaDecola->inicio->idVoo, 'D', hora, minuto, 3, filaDecola->inicio->prioridade);
				retiraFila(filaDecola);
				pista3 = 2; // 2 unidade de tempo decolagem
			}
		}

		if (pista1 > 0)
			pista1--;
		if (pista2 > 0)
			pista2--;
		if (pista3 > 0)
			pista3--;

		minuto+=5;
		if (minuto >= 60)
		{
			hora++;
			minuto = 0;
			if (hora >= 24)
				hora = 0;
		}

		if (unidadeTempo == 10)
		{
			Voo *aux2 = filaAproxima->inicio;
			while (aux2 != NULL)
			{
				aux2->prioridade = (aux2->prioridade) - 1;
				aux2 = aux2->prox;
			}
			unidadeTempo = 0;
		}

		Voo *aux3 = filaAproxima->inicio;
		while (aux3 != NULL)
		{
			if (aux3->prioridade < 0)
			{
				printf("\nALERTA CRÍTICO, AERONAVE IRÁ CAIR\n");
			}
			aux3 = aux3->prox;
		}
		unidadeTempo++;
	}

/*********************************************************************************************/

	free(filaAproxima);
	free(filaDecola);
	return 0;
}

Fila *criaFila()
{
  Fila *fila = (Fila *)malloc(sizeof(Fila));
  	if (fila == NULL)
  		exit(1);
  fila->inicio = NULL;
  fila->fim    = NULL;

  return fila;
}

void insereFila(Fila *fila, char *idVoo, char status, int prioridade)
{
  Voo *tmp = (Voo *)malloc(sizeof(Voo));
  
  strcpy(tmp->idVoo, idVoo);
  tmp->status = status;
  tmp->prioridade = prioridade;
  tmp->prox = NULL;
  if(!(fila->fim == NULL))
  {
    fila->fim->prox = tmp;
    fila->fim = tmp;
  }
  else
  {
    fila->inicio = fila->fim = tmp;
  }
}

void retiraFila(Fila *fila)
{
  Voo *tmp;

  if (fila->inicio == NULL)
  {
  	printf("Fila vazia!\n");
   	exit(1);
 	}
  tmp = fila->inicio;
  fila->inicio = fila->inicio->prox;
  if (fila->inicio == NULL)
  	fila->fim = NULL;
  
  free(tmp);
}

void imprimiFila(Voo *fila, char status)
{
	if (status == 'D')
	{
	  while (fila != NULL) 
	  {
	  	printf("[%s-%c]\n", fila->idVoo, fila->status);
	  	fila = fila->prox;
	  }
	}
	else if (status == 'A')
	{
	  while (fila != NULL) 
	  {
	  	printf("[%s-%c-%d]\n", fila->idVoo, fila->status, fila->prioridade);
	  	fila = fila->prox;
	  }
	}
}

void bubbleSort(Voo *fila)
{
	int swapped;
	Voo *ptr1, *lptr = NULL;

	if (fila == NULL)
		return;

	do
	{
		swapped = 0;
		ptr1 = fila;

		while(ptr1->prox != lptr)
		{
			if (ptr1->prioridade > ptr1->prox->prioridade)
			{
				swap(ptr1, ptr1->prox);
				swapped = 1;
			}
			ptr1 = ptr1->prox;
		}
		lptr = ptr1;
	}while(swapped);	
}

void swap(Voo *a, Voo *b)
{
	char tempIdVoo[7], tempStatus, tempPrioridade;

	strcpy(tempIdVoo, a->idVoo);
	tempStatus = a->status;
	tempPrioridade = a->prioridade;

	strcpy(a->idVoo, b->idVoo);
	a->status     = b->status;
	a->prioridade = b->prioridade;
	
	strcpy(b->idVoo, tempIdVoo);
	b->status     = tempStatus;
	b->prioridade = tempPrioridade;
}

void imprimiEvento(char *idVoo, char status, int hora, int minuto, int pista, int prioridade)
{
	if (status == 'A')
	{
		printf("Código do voo: %s - %d\n",idVoo, prioridade);
		printf("Status: Aeronave Pousou\n");
		printf("Horário do início do procedimento: %.2d:%.2d\n",hora,minuto);
		printf("Número da pista: %d\n\n", pista);
	}
	else if (status == 'D')
	{
		printf("Código do voo: %s\n",idVoo);
		printf("Status: Aeronave Decolou\n");
		printf("Horário do início do procedimento: %.2d:%.2d\n",hora,minuto);
		printf("Número da pista: %d\n\n", pista);
	}
}
