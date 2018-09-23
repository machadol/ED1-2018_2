#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TAMANHO_MAX_GLCM 256
#define VETOR_GLCM 24

void abreImagem(char[]);
void descobreTamanho(int *, int *);

FILE *arq;


int main(int argc, char const *argv[])
{
	int comp = 0, 
			larg = 0,
			num;
	char lixo;

	char imagemTeste []= "Nome da imagem";

	int *pComp,
			*pLarg,
			**imagem;

	pComp   	= &comp;
	pLarg   	= &larg;

	abreImagem(imagemTeste);
	descobreTamanho(pLarg,pComp);

	imagem = (int **) malloc((*pLarg)*sizeof(int*));
	if (imagem == NULL)
	{
		printf("Falha!\n");
		exit(1);
	}
	for (int i = 0; i < *pLarg; i++)
	{
		imagem[i] = (int *) malloc((*pComp)*sizeof(int));
		if (imagem[i] == NULL)
		{
			printf("Falha!\n");
			exit(1);
		}
	}
	rewind(arq);
		for (int i = 0; i < (*pLarg); i++)
		{
			for (int j = 0; j < (*pComp); j++)
			{
				fscanf(arq,"%d%c",&num,&lixo);
				imagem[i][j] = num;
			}
		}
	/****************************** Código de teste *****************************/


	/****************************************************************************/
	free(imagem);
	fclose(arq);
	return 0;
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
