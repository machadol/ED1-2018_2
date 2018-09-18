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
	int **numImg = sorteio(); // numImg[0] para grama.
                            // numImg[1] para asfalto.

	/***************** Variáveis *********************/
	int comp = 0, // Comprimento da imagem
			larg = 0; // Largura da imagem
	/*************************************************/

	/***************** Ponteiros *********************/
	int *pComp,
			*pLarg;

	pComp = &comp;
	pLarg = &larg;
	/*************************************************/

	/******** Imprimindo os numeros sorteados ********/	     
	printf("Números Grama\n");
	for (int i = 0; i < 50; i++)
	{
		if (i%10==0)
		{
			printf("\n");
		}
		printf(" %2d", numImg[0][i]);
	}
	printf("\n\nNúmeros Asfalto\n");
	for (int i = 0; i < 50; i++)
	{
		if (i%10==0)
		{
			printf("\n");
		}
		printf(" %2d", numImg[1][i]);
	}
	printf("\n\n");
	/*************************************************/

	/************** Chamada de funções ***************/
	// Função treinamento: 
	// 		Parâmetro tipo = 'G' para treinamento de imagens de grama.	
	//  								 = 'A' para treinamento de imagens de asfalto.
	treinamento('G',numImg[0],pLarg,pComp);
	treinamento('A',numImg[1],pLarg,pComp);
	/*************************************************/

	

	free(numImg);
	return 0;
}


int **sorteio()
{
	/************ Alocando Matriz 2x50 **************/
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
	/************************************************/

	/*********** Gerando os Números *****************/
	srand((int)time(NULL));
	int aux;
	for (int i = 0; i < 50; i++)
	{
		do
		{
			// Gera o número
			nums[0][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				// Verifica se o número gerado já existe
				// Se existir outro número será gerado.  
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
			// Gera o número
			nums[1][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				// Verifica se o número gerado já existe
				// Se existir outro número será gerado.  
				if (nums[1][i]==nums[1][j])
				{
					aux = 1;
					break;
				}
			}
		} while(aux);
	}
	/************************************************/
	
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
		// Conta a qtd de ';' somente na primeira linha.
		// Para encontrar a qtd de colunas(comprimento da img) na matriz
		if (linha == 0) 
		{
			if (ch == ';')
			{
				(*comp)++;
			}
		}
		// Conta a qtd de '\n' em todo o arquivo
		// Para encontrar a qtd de linhas(largura da img) na matriz
		if (ch == '\n')
		{
			(*larg)++;
			linha++;
		}
	}
	(*comp)++; // Corrigi a qtd de colunas
}

void treinamento(char tipo, int *numImg, int *larg, int *comp)
{	
	const char dirGrama  [] = "";
	const char dirAsfalto[] = "";
 	const char ext[] = ".txt";
	char imgGrama[25];
	char imgAsfalto[30];

	if (tipo == 'G')
	{
		for (int i = 0; i < 25; ++i)
		{
			// Concatena o número sorteado ao diretório da img
			sprintf(imgGrama, "grass/grass_%.2d.txt", numImg[i]);
			printf("img: %s\n", imgGrama);

 			abreImagem(imgGrama);
 			descobreTamanho(larg,comp);

 			printf("Comp = %d\nLarg = %d\n\n",*comp, *larg);

 			// Zera a largura e o comprimento para a próxima img
 			*larg = 0;
 			*comp = 0;
 			// Fecha o arquivo
			fclose(arq);
		}
	}
	else if(tipo == 'A')
	{
		for (int i = 0; i < 25; ++i)
		{
			// Concatena o número sorteado ao diretório da img
			sprintf(imgAsfalto, "asphalt/asphalt_%.2d.txt", numImg[i]);
			printf("img: %s\n", imgAsfalto);

 			abreImagem(imgAsfalto);
 			descobreTamanho(larg,comp);

 			printf("Comp = %d\nLarg = %d\n\n",*comp, *larg);

 			// Zera a largura e o comprimento para a próxima img
 			*larg = 0;
 			*comp = 0;
 			// Fecha o arquivo
 			fclose(arq);
		}
	}
}