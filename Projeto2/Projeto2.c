#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define QTD_AMOSTRAS 25
#define TAMANHO_MAX_GLCM 256
#define VETOR_GLCM 24
#define ILBP_VET 512
#define GLCM_VET 24
#define RESULTADO_VET 536

void abreImagem(char[]);
void descobreTamanho(int *, int *);

void ilbp(int **, double *, int *, int *);

void glcmDireita (int **, double *);
void glcmEsquerda(int **, double *);
void glcmSuperior(int **, double *);
void glcmInferior(int **, double *);
void glcmSuperiorDireito (int **, double *);
void glcmSuperiorEsquerdo(int **, double *);
void glcmInferiorDireita (int **, double *);
void glcmInferiorEsquerda(int **, double *);
void glcmCompleta (int **, double *);
void glcmContraste(int **, double *);
void glcmEnergia  (int **, double *);
void glcmHomogeneidade(int **, double *);

void zeraILBPeGLSM (double *, double *);
void descobreMaxMin(double *, double *, double *);
void normaliza 		 (double *, double *, double *);
int menorbinario(int **);

FILE *arq;


int main(int argc, char const *argv[])
{
	/***************** Variáveis *********************/
	int comp = 0, // Comprimento da imagem
			larg = 0, // Largura da imagem
		  numPixel; // número referente ao pixel da imagem.
	double max,
				 min;
	char lixo, 	  // auxilia na leitura do ';' e '\n'
			 imgGrama[25],
			 imgAsfalto[30];
 	
	/***************** Ponteiros *********************/
	int **numImg, // numImg[0][24] para grama. numImg[1][24] para asfalto.
			**imagem,
		  *pComp,
			*pLarg;
	double **resultadoTreinamentoGrama,
				 **resultadoTreinamentoAsfalto,
				 **resultadoTesteGrama,
				 **resultadoTesteAsfalto,
				 *normal,
				 *ilbpVet,
				 *glcmVet,
		  	 *pMax,
				 *pMin;

	pComp = &comp;
	pLarg = &larg;
	pMax	= &max;
	pMin  = &min;
	/****************** Alocando ********************/
	ilbpVet = (double *)calloc(ILBP_VET,sizeof(double));
		if (ilbpVet == NULL)
		{
			printf("Falha! CALLOC ilbpVet\n");
			exit(1);
		}
	glcmVet = (double *)calloc(GLCM_VET,sizeof(double));
		if (glcmVet == NULL)
		{
			printf("Falha! CALLOC glcmVet\n");
			exit(1);
		}
	normal  = (double *)calloc(RESULTADO_VET, sizeof(double));
		if (normal == NULL)
		{
			printf("Falha! CALLOC normal\n");
			exit(1);
		}


	numImg = (int **) malloc(2*sizeof(int*));
		if (numImg == NULL)
		{
			printf("Falha! MALLOC numImg\n");
			exit(1);
		}
		for (int i = 0; i < 2; ++i)
		{
			numImg[i] = (int *) malloc(50*sizeof(int));
				if (numImg[i] == NULL)
				{
					printf("Falha! MALLOC numImg[%d]\n",i);
					exit(1);
				}
		}
	resultadoTreinamentoGrama   = (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
	resultadoTreinamentoAsfalto = (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
	resultadoTesteGrama					= (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
	resultadoTesteAsfalto				= (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
		if (resultadoTreinamentoGrama == NULL)
		{
			printf("Falha! CALLOC resultadoTreinamentoGrama\n");
			exit(1);
		} 
		if (resultadoTreinamentoAsfalto == NULL)
		{
			printf("Falha! CALLOC resultadoTreinamentoAsfalto\n");
			exit(1);
		} 
		if (resultadoTesteGrama == NULL)
		{
			printf("Falha! CALLOC resultadoTesteGrama\n");
			exit(1);
		} 
		if (resultadoTesteAsfalto == NULL)
		{
			printf("Falha! CALLOC resultadoTesteAsfalto\n");
			exit(1);
		}
		for (int i = 0; i < QTD_AMOSTRAS; i++)
		{
			resultadoTreinamentoGrama[i]   = (double *)calloc(RESULTADO_VET,sizeof(double));
			resultadoTreinamentoAsfalto[i] = (double *)calloc(RESULTADO_VET,sizeof(double));
			resultadoTesteGrama[i]				 = (double *)calloc(RESULTADO_VET,sizeof(double));
			resultadoTesteAsfalto[i]			 = (double *)calloc(RESULTADO_VET,sizeof(double));
				if (resultadoTreinamentoGrama[i] == NULL)
				{
					printf("Falha! CALLOC resultadoTreinamentoGrama[%d]\n",i);
					exit(1);
				} 
				if (resultadoTreinamentoAsfalto[i] == NULL)
				{
					printf("Falha! CALLOC resultadoTreinamentoAsfalto[%d]\n",i);
					exit(1);
				} 
				if (resultadoTesteGrama[i] == NULL)
				{
					printf("Falha! CALLOC resultadoTesteGrama[%d]\n",i);
					exit(1);
				} 
				if (resultadoTesteAsfalto[i] == NULL)
				{
					printf("Falha! CALLOC resultadoTesteAsfalto[%d]\n",i);
					exit(1);
				}
		} 

	/*********** Sorteando os Números ***************/
	srand((int)time(NULL));
	int aux;
	for (int i = 0; i < 50; i++)
	{
		do
		{
			// Gera o número
			numImg[0][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				// Verifica se o número gerado já existe
				// Se existir outro número será gerado.
				if (numImg[0][i]==numImg[0][j])
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
			numImg[1][i] = rand()%50 + 1;
			aux = 0;

			for (int j = 0; j < i; j++)
			{
				// Verifica se o número gerado já existe
				// Se existir outro número será gerado.
				if (numImg[1][i]==numImg[1][j])
				{
					aux = 1;
					break;
				}
			}
		} while(aux);
	}

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

	/*************** Treinamento Grama ****************/
	for (int i = 0; i < QTD_AMOSTRAS; i++)
	{
		// Concatena o número sorteado ao diretório da img
		sprintf(imgGrama, "grass/grass_%.2d.txt", numImg[0][i]);
		printf("\nimg: grass_%.2d\n", numImg[0][i]);
		
		abreImagem(imgGrama);
 		descobreTamanho(pLarg,pComp);

 		// Alocando Matriz da imagem 
		imagem = (int **) malloc((*pLarg)*sizeof(int *));
			if (imagem == NULL)
			{
				printf("Falha! MALLOC imagem\n");
				exit(1);
			}
			for (int i = 0; i < (*pComp); i++)
			{
				imagem[i] = (int *) malloc((*pComp)*sizeof(int));
				if (imagem[i] == NULL)
				{
					printf("Falha! MALLOC imagem[%d]\n",i);
					exit(1);
				}
			}
			rewind(arq);
			for (int i = 0; i < (*pLarg); i++)
			{
				for (int j = 0; j < (*pComp); j++)
				{
					fscanf(arq,"%d%c",&numPixel,&lixo);
					imagem[i][j] = numPixel;
				}
			}

		ilbp(imagem,ilbpVet,pLarg,pComp);
		glcmCompleta(imagem,glcmVet);

		// Concatena ILBP com GLCM
		for (int j = 0; j < ILBP_VET; j++)
		{
			normal[j] = ilbpVet[j];
		}
		for (int j = ILBP_VET, k = 0; j < RESULTADO_VET; j++,k++)
		{
			normal[j] = glcmVet[k];
		}

		for (int i = 0; i < RESULTADO_VET; i++)
		{
			printf("%.10lf ",normal[i]);
		}
		printf("\n\n\n");
		descobreMaxMin(pMax,pMin,normal);
		printf("max %lf min %lf \n", *pMax, *pMin);
		normaliza(pMax,pMin,normal);

		for (int i = 0; i < RESULTADO_VET; i++)
		{
			printf("%.10lf ",normal[i]);
		}

 		// Liberando Memória
 		for (int i = 0; i < (*pLarg); i++)
 		{
 			free(imagem[i]);
 		}
 		free(imagem);
 		// Zera parâmetros
 		*pLarg = 0;
 		*pComp = 0;
		zeraILBPeGLSM(ilbpVet,glcmVet);
 		// Fecha o arquivo
		fclose(arq);
	}

	// /*************** Treinamento Asfalto ****************/
	// for (int i = 0; i < QTD_AMOSTRAS; i++)
	// {
	// 	// Concatena o número sorteado ao diretório da img
	// 	sprintf(imgGrama, "asphalt/asphalt_%.2d.txt", numImg[1][i]);
	// 	printf("img: asphalt_%.2d\n", numImg[1][i]);
		
	// 	abreImagem(imgGrama);
 // 		descobreTamanho(pLarg,pComp);

 // 		// Alocando Matriz da imagem 
	// 	imagem = (int **) malloc((*pLarg)*sizeof(int *));
	// 		if (imagem == NULL)
	// 		{
	// 			printf("Falha MALLOC.\n");
	// 			exit(1);
	// 		}
	// 		for (int i = 0; i < (*pComp); i++)
	// 		{
	// 			imagem[i] = (int *) malloc((*pComp)*sizeof(int));
	// 			if (imagem[i] == NULL)
	// 			{
	// 				printf("Falha MALLOC.\n");
	// 				exit(1);
	// 			}
	// 		}
	// 		rewind(arq);
	// 		for (int i = 0; i < (*pLarg); i++)
	// 		{
	// 			for (int j = 0; j < (*pComp); j++)
	// 			{
	// 				fscanf(arq,"%d%c",&numPixel,&lixo);
	// 				imagem[i][j] = numPixel;
	// 			}
	// 		}

	// 	ilbp(imagem,ilbpVet,pLarg,pComp);

 // 		// Liberando Memória
 // 		for (int i = 0; i < (*pLarg); i++)
 // 		{
 // 			free(imagem[i]);
 // 		}
 // 		free(imagem);
 // 		// Zera parâmetros
 // 		*pLarg = 0;
 // 		*pComp = 0;
	// 	zeraILBPeGLSM(ilbpVet,glcmVet);
 // 		// Fecha o arquivo
	// 	fclose(arq);
	// }
	
	/*************** Liberando Memória ****************/
	for (int i = 0; i < 2; i++)
		free(numImg[i]);
	free(numImg);

	for (int i = 0; i < QTD_AMOSTRAS; i++)
	{
		free(resultadoTreinamentoGrama[i]);
		free(resultadoTreinamentoAsfalto[i]);
		free(resultadoTesteGrama[i]);
		free(resultadoTesteAsfalto[i]);
	} 
	free(resultadoTreinamentoGrama);
	free(resultadoTreinamentoAsfalto);
	free(resultadoTesteGrama);
	free(resultadoTesteAsfalto);

	free(ilbpVet);
	free(glcmVet);
	free(normal);

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

void ilbp(int **imagem, double *ilbpVet, int *larg, int *comp)
{
	int media,menorDecimal;
  int **matrizImg; //3x3 percorre imagem
  int **matrizBin; //3x3 binário

  matrizImg = (int **)calloc(3, sizeof(int *));
  matrizBin = (int **)calloc(3, sizeof(int *));
  if (matrizImg == NULL || matrizBin == NULL)
  {
  	printf("Falha! CALLOC\n");
  	exit(1);
  }
  	for (int i = 0; i < 3; i++)
		{
			matrizImg[i] = (int *)calloc(3, sizeof(int));
			matrizBin[i] = (int *)calloc(3, sizeof(int));
		  if (matrizImg[i] == NULL || matrizBin[i] == NULL)
		  {
		  	printf("Falha! CALLOC\n");
		  	exit(1);
		  }
		}

	for (int i = 1; i < (*larg - 3); i++)
	{
		for (int j = 1 ; j < (*comp - 3); j++)
		{
			matrizImg[0][0] = imagem[i][j];
			matrizImg[0][1] = imagem[i][j+1];
			matrizImg[0][2] = imagem[i][j+2];
			matrizImg[1][0] = imagem[i+1][j];
			matrizImg[1][1] = imagem[i+1][j+1];
			matrizImg[1][2] = imagem[i+1][j+2];
			matrizImg[2][0] = imagem[i + 2][j];
			matrizImg[2][1] = imagem[i + 2][j+1];
			matrizImg[2][2] = imagem[i + 2][j+2];
		  // Percorrer a matriz
		  media = 0;
		  for(int i=0; i<3; i++ )
		  {
		  	for (int j= 0; j<3; j++)
		   	{
		   		media+= matrizImg[i][j];
		   	}
		  }
		  media = media/9;
			
			// Normatizando o vet
		  for(int i= 0; i<3; i++)
		  {
		  	for(int j=0; j<3; j++)
		  	{
		  		if(matrizImg[i][j] > media)
		  			matrizBin[i][j] = 1;
          else
          	matrizBin[i][j] = 0;
        }
   		}
	    menorDecimal = menorbinario(matrizBin);
	    ilbpVet[menorDecimal]++;
		}
	}

	for (int i = 0; i < 3; i++) 
	{
		free(matrizImg[i]); 
		free(matrizBin[i]);
	}
  	free(matrizImg);
  	free(matrizBin);
}

int menorbinario(int **matrizAux)
{
	int *v,*min, i, k , decimal, a;
	
	v = (int *)calloc(9, sizeof(int));
  min = (int *)calloc(9, sizeof(int));

  v[0]= matrizAux[0][0];
  v[1]= matrizAux[0][1];
  v[2]= matrizAux[0][2];
  v[3]= matrizAux[1][2];
  v[4]= matrizAux[2][2];
  v[5]= matrizAux[2][1];
  v[6]= matrizAux[2][0];
  v[7]= matrizAux[1][0];
  v[8]= matrizAux[1][1];

  k=0;
  while(k<10)
  {
  	decimal = 0;
   	i= 0;
   	for(int j = 8; j >= 0; j--)
   	{
   		decimal+= v[i]*pow(2,j);
      i++;
    }
    a= v[8];
    for(int j = 8; j >= 1; j--)
    {
    	v[j]= v[j-1];
    }
    v[0]=a;
		min[k]=decimal;
    k++;
  }
	decimal = min[0];
  
  for (int j = 1; j < 10;++j)
   	if (min[j] < decimal)
   	{
   		decimal = min[j];
    }

  free(v);
  free(min);
	return decimal;
}

// Matriz GLCM Direita
void glcmDireita(int **imagem, double *resultado)
{
	int **glcm_Direita;

	glcm_Direita = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_Direita == NULL)
		{
			printf("Falha! CALLOC glcm_Direita\n");
			exit(1);
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_Direita[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_Direita[i] == NULL)
			{
				printf("Falha! CALLOC glcm_Direita[%d]\n",i);
				exit(1);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if (j != TAMANHO_MAX_GLCM)
			{
				glcm_Direita[imagem[i][j]][imagem[i][j + 1]]++;
			}
		}
	}

	glcmContraste(glcm_Direita, resultado);
	glcmEnergia(glcm_Direita, resultado);
	glcmHomogeneidade(glcm_Direita, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_Direita[i]);
	}

	free(glcm_Direita);
}

//Matriz GLCM Esquerda
void glcmEsquerda(int **imagem, double *resultado)
{
	int **glcm_Esquerda;

	glcm_Esquerda = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_Esquerda == NULL)
		{
			printf("Falha! CALLOC glcm_Esquerda\n");
			exit(1);
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_Esquerda[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_Esquerda[i] == NULL)
			{
				printf("Falha! CALLOC glcm_Esquerda[%d]\n",i);
				exit(1);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if (j != 0)
			{
				glcm_Esquerda[imagem[i][j]][imagem[i][j - 1]]++;
			}
		}
	}

	glcmContraste(glcm_Esquerda, resultado);
	glcmEnergia(glcm_Esquerda, resultado);
	glcmHomogeneidade(glcm_Esquerda, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_Esquerda[i]);
	}

	free(glcm_Esquerda);
}

//Matriz GLCM Superior
void glcmSuperior(int **imagem, double *resultado)
{
	int **glcm_Superior;

	glcm_Superior = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_Superior == NULL)
		{
			printf("Falha! CALLOC glcm_Superior\n");
			exit(1);
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_Superior[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_Superior[i] == NULL)
			{
				printf("Falha! CALLOC glcm_Superior[%d]\n",i);
				exit(1);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if (i != 0)
			{
        glcm_Superior[imagem[i - 1][j]][imagem[i][j]]++;
			}
		}
	}

	glcmContraste(glcm_Superior, resultado);
	glcmEnergia(glcm_Superior, resultado);
	glcmHomogeneidade(glcm_Superior, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_Superior[i]);
	}

	free(glcm_Superior);
}

//Matriz GLCM Inferior
void glcmInferior(int **imagem, double *resultado)
{
	int **glcm_Inferior;

	glcm_Inferior = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_Inferior == NULL)
		{
			printf("Falha! CALLOC glcm_Inferior\n");
			exit(1);
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_Inferior[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_Inferior[i] == NULL)
			{
				printf("Falha! CALLOC glcm_Inferior[%d]\n",i);
				exit(1);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if (i != TAMANHO_MAX_GLCM)
			{
				glcm_Inferior[imagem[i + 1][j]][imagem[i][j]]++;
			}
		}
	}

	glcmContraste(glcm_Inferior, resultado);
	glcmEnergia(glcm_Inferior, resultado);
	glcmHomogeneidade(glcm_Inferior, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_Inferior[i]);
	}

	free(glcm_Inferior);
}

//Matriz GLCM Superior Direito
void glcmSuperiorDireito(int **imagem, double *resultado)
{
	int **glcm_SuperiorDireito;

	glcm_SuperiorDireito = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_SuperiorDireito == NULL)
		{
			printf("Falha! CALLOC glcm_SuperiorDireito\n");
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_SuperiorDireito[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_SuperiorDireito[i] == NULL)
			{
				printf("Falha! CALLOC glcm_SuperiorDireito[%d]\n",i);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if ((i != 0) && (j != TAMANHO_MAX_GLCM))
			{
				glcm_SuperiorDireito[imagem[i - 1][j]][imagem[i][j + 1]]++;
			}
		}
	}

	glcmContraste(glcm_SuperiorDireito, resultado);
	glcmEnergia(glcm_SuperiorDireito, resultado);
	glcmHomogeneidade(glcm_SuperiorDireito, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_SuperiorDireito[i]);
	}

	free(glcm_SuperiorDireito);
}

//Matriz GLCM Superior Esquerdo
void glcmSuperiorEsquerdo(int **imagem, double *resultado)
{

	int **glcm_SuperiorEsquerdo;

	glcm_SuperiorEsquerdo = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_SuperiorEsquerdo == NULL)
		{
			printf("Falha! CALLOC glcm_SuperiorEsquerdo\n");
			exit(1);
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_SuperiorEsquerdo[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_SuperiorEsquerdo[i] == NULL)
			{
				printf("Falha! CALLOC glcm_SuperiorEsquerdo[%d]\n",i);
				exit(1);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if ((i != 0) && (j != 0))
			{
				glcm_SuperiorEsquerdo[imagem[i - 1][j]][imagem[i][j - 1]]++;
			}
		}
	}

	glcmContraste(glcm_SuperiorEsquerdo, resultado);
	glcmEnergia(glcm_SuperiorEsquerdo, resultado);
	glcmHomogeneidade(glcm_SuperiorEsquerdo, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++){
		free(glcm_SuperiorEsquerdo[i]);
	}

	free(glcm_SuperiorEsquerdo);
}

//Matriz GLCM Inferior Direita
void glcmInferiorDireita(int **imagem, double *resultado)
{
	int **glcm_InferiorDireita;

	glcm_InferiorDireita = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_InferiorDireita == NULL)
		{
			printf("Falha! CALLOC glcm_InferiorDireita\n");
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_InferiorDireita[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_InferiorDireita[i] == NULL)
			{
				printf("Falha! CALLOC glcm_InferiorDireita[%d]\n",i);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if ((i != TAMANHO_MAX_GLCM) && (j != TAMANHO_MAX_GLCM))
			{
				glcm_InferiorDireita[imagem[i + 1][j]][imagem[i][j + 1]]++;
			}
		}
	}
	
	glcmContraste(glcm_InferiorDireita, resultado);
	glcmEnergia(glcm_InferiorDireita, resultado);
	glcmHomogeneidade(glcm_InferiorDireita, resultado);
	
	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_InferiorDireita[i]);
	}
		free(glcm_InferiorDireita);
}

//Matriz GLCM Inferior Esquerda
void glcmInferiorEsquerda(int **imagem, double *resultado)
{
	int **glcm_InferiorEsquerda;

	glcm_InferiorEsquerda = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
		if (glcm_InferiorEsquerda == NULL)
		{
			printf("Falha! CALLOC glcm_InferiorEsquerda\n");
		}
		for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
		{
			glcm_InferiorEsquerda[i] = (int *)calloc(TAMANHO_MAX_GLCM, sizeof(int));
			if (glcm_InferiorEsquerda[i] == NULL)
			{
				printf("Falha! CALLOC glcm_InferiorEsquerda[%d]\n",i);
			}
		}

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			if ((i != TAMANHO_MAX_GLCM) && (j != 0))
			{
				glcm_InferiorEsquerda[imagem[i + 1][j]][imagem[i][j - 1]]++;
			}
		}
	}

	glcmContraste(glcm_InferiorEsquerda, resultado);
	glcmEnergia(glcm_InferiorEsquerda, resultado);
	glcmHomogeneidade(glcm_InferiorEsquerda, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_InferiorEsquerda[i]);
	}
	free(glcm_InferiorEsquerda);
}

void glcmCompleta(int **imagem, double *resultado)
{

    glcmDireita (imagem, resultado);
    glcmEsquerda(imagem, resultado);
    glcmSuperior(imagem, resultado);
    glcmInferior(imagem, resultado);
    glcmSuperiorDireito (imagem, resultado);
    glcmSuperiorEsquerdo(imagem, resultado);
    glcmInferiorDireita (imagem, resultado);
    glcmInferiorEsquerda(imagem, resultado);
}

void glcmContraste(int **matrizGLCM, double *resultContraste)
{
	double contraste = 0;

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			contraste += pow(abs(i-j),2)*matrizGLCM[i][j];
		}
	}
	for (int i = 0; i < VETOR_GLCM; ++i)
	{
		if(resultContraste[i] == 0)
		{
			resultContraste[i] = contraste;
			break;
		}
	}
}

void glcmEnergia(int **matrizGLCM, double *resultEnergia)
{
	double energia = 0;

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			energia += pow(matrizGLCM[i][j],2);
		}
	}
	for (int i = 0; i < VETOR_GLCM; i++)
	{
		if (resultEnergia[i] == 0)
		{
			resultEnergia[i] = energia;
			break;
		}
	}
}

void glcmHomogeneidade(int **matrizGLCM, double *resultHomogeneidade)
{
	double homogeneidade;

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			homogeneidade += matrizGLCM[i][j]/(1 + abs(i-j));
		}
	}
	for (int i = 0; i < VETOR_GLCM; i++)
	{
		if (resultHomogeneidade[i] == 0)
		{
			resultHomogeneidade[i] = homogeneidade;
		}
	}
}

void zeraILBPeGLSM(double *ilbp, double *glcm)
{
	for (int i = 0; i < ILBP_VET; ++i)
		ilbp[i] = 0;
	for (int i = 0; i < GLCM_VET; ++i)
		glcm[i] = 0;
}

void descobreMaxMin(double *max, double *min, double *vetor)
{
	int tempMax = 0;
	int tempMin = 0;

	for (int i = 0; i < RESULTADO_VET; i++)
	{
		if (*(vetor + i) > tempMax)
		{
			tempMax = *(vetor + i);
		}
		else if (*(vetor + i) < tempMin)
		{
			tempMin = *(vetor + i);
		}
	}
	*max = tempMax;
	*min = tempMin;
}

void normaliza(double *max, double *min, double *vetor)
{
	for (int i = 0; i < RESULTADO_VET; i++)
	{
		*(vetor + i) = ((double)(*(vetor + i) - *min) / (*max - *min));
	}
}