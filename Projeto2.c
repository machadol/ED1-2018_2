#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int **sorteio();
void abreImagem(char[]);
void descobreTamanho(int *, int *);
void treinamento(char, int *, int *, int *);

FILE *arq;


int main(int argc, char const *argv[])
{
	// 0 = Grama 1 = Asfalto
	int **numImg = sorteio();
	
	printf("Números grama\n");
	for (int i = 0; i < 50; i++)
	{
		if (i%10==0)
		{
			printf("\n");
		}
		printf(" %2d", numImg[0][i]);
	}
	printf("\n\n");

	printf("Números Asfalto\n");
	for (int i = 0; i < 50; i++)
	{
		if (i%10==0)
		{
			printf("\n");
		}
		printf(" %2d", numImg[1][i]);
	}
	printf("\n\n");


	int comp = 0, 
			larg = 0;

	int *pComp,
			*pLarg;

	pComp = &comp;
	pLarg = &larg;

	treinamento('G',numImg[0],pLarg,pComp);
	treinamento('A',numImg[1],pLarg,pComp);

	free(numImg);
	return 0;
}


int **sorteio()
{
	int **nums = (int **) malloc(2*sizeof(int));
		if (nums == NULL)
		{
			printf("Falha! MALLOC.\n");
			exit(1);
		}
	for (int i = 0; i < 2; ++i)
	{
		nums[i] = (int *) malloc(50*sizeof(int));
			if (nums[i] == NULL)
			{
				printf("Falha! MALLOC.\n");
				exit(1);
			}
	} 

	srand((int)time(NULL));

	int aux;

	for (int i = 0; i < 50; i++)
	{
		do
		{
			nums[0][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				if (nums[0][i]==nums[0][j])
				{
					aux = 1;
					break;
				}
			}
		} while(aux);
	}
	for (int i = 0; i < 50; i++)
	{
		do
		{
			nums[1][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				if (nums[1][i]==nums[1][j])
				{
					aux = 1;
					break;
				}
			}
		} while(aux);
	}

	return nums;
}

void abreImagem(char nomeImagem[])
{
	arq = fopen(nomeImagem,"r");
	if (arq==NULL)
	{
		printf("Falha ao abrir a imagem.\n");
		exit(1);
	}
}

void descobreTamanho(int *larg, int *comp)
{
	char ch;
	int linha = 0;

	while((ch=getc(arq)) != EOF)
	{
		if (linha == 0)
		{
			if (ch == ';')
			{
				(*comp)++;
			}
		}
		if (ch == '\n')
		{
			(*larg)++;
			linha++;
		}
	}
	(*comp)++;
}

void treinamento(char tipo, int *numImg, int *larg, int *comp)
{	
	const char dirGrama  [] = "grass/grass_";
	const char dirAsfalto[] = "asphalt/asphalt_";
 	const char ext[] = ".txt";
	char imgGrama[25];
	char imgAsfalto[30];

	if (tipo == 'G')
	{
		for (int i = 0; i < 25; ++i)
		{
			if (numImg[i] <= 9)
			{
				sprintf(imgGrama, "%s0%d%s", dirGrama, numImg[i], ext);
				printf("img = %s", imgGrama);
				printf("\n");
			}
			else
			{
				sprintf(imgGrama, "%s%d%s", dirGrama, numImg[i], ext);
				printf("img = %s", imgGrama);
				printf("\n");
			}
 			
 			abreImagem(imgGrama);
 			descobreTamanho(larg,comp);
 			printf("Comprimento = %d\nLargura = %d\n",*comp, *larg);
 			printf("\n");
 			*larg = 0;
 			*comp = 0;
			fclose(arq);
		}
	}
	else if(tipo == 'A')
	{
		for (int i = 0; i < 25; ++i)
		{
			if (numImg[i] <= 9)
			{
				sprintf(imgAsfalto, "%s0%d%s", dirAsfalto, numImg[i], ext);
				printf("img = %s", imgAsfalto);
				printf("\n");
			}
			else
			{
				sprintf(imgAsfalto, "%s%d%s", dirAsfalto, numImg[i], ext);
				printf("img = %s", imgAsfalto);
				printf("\n");
			}
 			
 			abreImagem(imgAsfalto);
 			descobreTamanho(larg,comp);
 			printf("Comprimento = %d\nLargura = %d\n",*comp, *larg);
 			printf("\n");
 			*larg = 0;
 			*comp = 0;
 			fclose(arq);
		}
	}
}