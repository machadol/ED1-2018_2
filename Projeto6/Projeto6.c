#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define QTD_AMOSTRAS 50
#define TAMANHO_MAX_GLCM 256
#define VETOR_GLCM 24
#define ILBP_VET 512
#define GLCM_VET 24
#define RESULTADO_VET 536
#define LEARNING_RATE 1

typedef struct
{	
	double b;
  double *w;
  double grad;
  double s;
  int tamW;
}NEURONIO;

void abreImagem(char[]);
void descobreTamanho(int *, int *);
void ilbp(int **, double *, int *, int *);
void glcm(int **, double *);
void glcmMetricas(int **, double *);
void zeraILBPeGLSM(double *, double *);
void descobreMaxMin(double *, double *, double *);
void normaliza(double *, double *, double *);
void calculaMedia(double **, double *);
void distEuclidiana(double *, double *, double **, double **);
int  comparaDistancia(double , double);
int  menorbinario(int **);

NEURONIO* alocaNeuronios(int);
void pegaValorEntrada(double *, double *, int *);
void readFromFile(double *, char);
double dxSigmoide(double);
double valorMedio(NEURONIO*, double*, int);
double sigmoide (double);
double* alocaPeso(int);
void backPropagation(NEURONIO *, NEURONIO *, NEURONIO *, double *, int, double);
double feedFoward(NEURONIO *, NEURONIO *, NEURONIO *, double *, double, int, double *);

void randomizaPeso(NEURONIO *,int);
double erroQuadMed(double*);

FILE *arq;

int main(int argc, char const *argv[])
{
	/***************** Variáveis *********************/
	int comp = 0, // Comprimento da imagem
			larg = 0, // Largura da imagem
		  numPixel; // número referente ao pixel da imagem.
	double max,
				 min,
				 taxaAcerto = 0,
				 falsaAceitacao = 0,
				 falsaRejeicao = 0;
	char lixo, 	  // auxilia na leitura do ';' e '\n'
			 imgGrama[25],
			 imgAsfalto[30];
 	
	/***************** Ponteiros *********************/
	int **numImg, // numImg[0][24] para grama. numImg[1][24] para asfalto.
			**imagem,
		  *pComp,
			*pLarg;
	double **resultadoGrama,
				 **resultadoAsfalto,
				 *mediaGrama,
				 *mediaAsfalto,
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
	mediaGrama   = (double *)calloc(RESULTADO_VET,sizeof(double));
		if (mediaGrama == NULL)
		{
			printf("Falha! CALLOC mediaGrama\n");
			exit(1);
		}
	mediaAsfalto = (double *)calloc(RESULTADO_VET,sizeof(double));
		if (mediaAsfalto == NULL)
		{
			printf("Falha! CALLOC mediaAsfalto\n");
			exit(1);
		}
	numImg    = (int **) calloc(2,sizeof(int*));
		if (numImg == NULL)
		{
			printf("Falha! CALLOC numImg\n");
			exit(1);
		}
		for (int i = 0; i < 2; ++i)
		{
			numImg[i] = (int *) calloc(50,sizeof(int));
				if (numImg[i] == NULL)
				{
					printf("Falha! CALLOC numImg[%d]\n",i);
					exit(1);
				}
		}
	resultadoGrama   = (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
	resultadoAsfalto = (double **)calloc(QTD_AMOSTRAS,sizeof(double *));
		if (resultadoGrama == NULL)
		{
			printf("Falha! CALLOC resultadoGrama\n");
			exit(1);
		} 
		if (resultadoAsfalto == NULL)
		{
			printf("Falha! CALLOC resultadoAsfalto\n");
			exit(1);
		} 
		for (int i = 0; i < QTD_AMOSTRAS; i++)
		{
			resultadoGrama[i]				 = (double *)calloc(RESULTADO_VET,sizeof(double));
			resultadoAsfalto[i]			 = (double *)calloc(RESULTADO_VET,sizeof(double));
				if (resultadoGrama[i] == NULL)
				{
					printf("Falha! CALLOC resultadoGrama[%d]\n",i);
					exit(1);
				} 
				if (resultadoAsfalto[i] == NULL)
				{
					printf("Falha! CALLOC resultadoAsfalto[%d]\n",i);
					exit(1);
				}
		} 

	/*********** Sorteando os Números ***************/
	int aux;
	srand((int)time(NULL));
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

 /*******************************************************************************************/ 

	/************** Processando Imagens para Grama ****************/
	for (int i = 0; i < QTD_AMOSTRAS; i++)
	{
		// Concatena o número sorteado ao diretório da img
		sprintf(imgGrama, "grass/grass_%.2d.txt", numImg[0][i]);
		printf("img %.2d: grass_%.2d\n",i+1, numImg[0][i]);
		
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
		glcm(imagem,glcmVet);

		// Concatena ILBP com GLCM
		for (int j = 0; j < ILBP_VET; j++)
		{
			normal[j] = ilbpVet[j];
		}
		for (int j = ILBP_VET, k = 0; j < RESULTADO_VET; j++,k++)
		{
			normal[j] = glcmVet[k];
		}

		// Normalização
		descobreMaxMin(pMax,pMin,normal);
		normaliza(pMax,pMin,normal);

		// Armazenando resuldao final do treinamento
		for (int j = 0; j < RESULTADO_VET; j++)
		{
			resultadoGrama[i][j] = normal[j];
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

	/************** Processando Imagens para Asfalto **************/
	for (int i = 0; i < QTD_AMOSTRAS; i++)
	{
		// Concatena o número sorteado ao diretório da img
		sprintf(imgGrama, "asphalt/asphalt_%.2d.txt", numImg[1][i]);
		printf("img %.2d: asphalt_%.2d\n", i+1, numImg[1][i]);
		
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
		glcm(imagem,glcmVet);

		// Concatena ILBP com GLCM
		for (int j = 0; j < ILBP_VET; j++)
		{
			normal[j] = ilbpVet[j];
		}
		for (int j = ILBP_VET, k = 0; j < RESULTADO_VET; j++,k++)
		{
			normal[j] = glcmVet[k];
		}

		// Normalização
		descobreMaxMin(pMax,pMin,normal);
		normaliza(pMax,pMin,normal);

		// Armazenando resuldao final do treinamento
		for (int j = 0; j < RESULTADO_VET; j++)
		{
			resultadoAsfalto[i][j] = normal[j];
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

 /*******************************************************************************************/
 	
	int tamCamadaOculta;
  double error[50];
  double classificacao;

  NEURONIO cEntrada[RESULTADO_VET];
  NEURONIO *cOculta;
  NEURONIO cSaida;

  tamCamadaOculta = atoi(argv[1]);

  cOculta = alocaNeuronios(tamCamadaOculta);

  for (int i = 0; i < RESULTADO_VET; i++)
  	cEntrada[i].w = alocaPeso(RESULTADO_VET);
  
  for (int i = 0; i < tamCamadaOculta; i++)
  	cOculta[i].w = alocaPeso(RESULTADO_VET);
  
  cSaida.w = alocaPeso(tamCamadaOculta);

	int freqGrass[50] = {};
  int freqAsphalt[50] = {};
  double inputP[RESULTADO_VET];

  int acerto_Asphalt = 0;
  int acerto_Grass = 0;

  for (int i = 0; i < RESULTADO_VET; i++)
  	cEntrada[i].tamW = RESULTADO_VET;
    
  for (int i = 0; i < tamCamadaOculta; i++)
  	cOculta[i].tamW = RESULTADO_VET;

  
  cOculta = alocaNeuronios(tamCamadaOculta);

  randomizaPeso(cEntrada,RESULTADO_VET);
  randomizaPeso(cOculta,tamCamadaOculta);
  randomizaPeso(&cSaida,1);
	
	double erroEp = 1;
  for (int epoca = 0;(epoca < 1000)&&(erroEp > 0.20f);epoca++)
  {
  	for (int feature = 0;feature < 50;feature+=2)
  	{
  		pegaValorEntrada(inputP,&resultadoAsfalto[0][0],freqAsphalt);
      error[feature] = feedFoward(cEntrada,
      													  cOculta,
      													  &cSaida,
      													  inputP,
      													  0.0f,
      													  tamCamadaOculta,
      													  &classificacao);
      
      backPropagation(cEntrada,cOculta,&cSaida,inputP,tamCamadaOculta,error[feature]);
      if (classificacao <= 0.5f) acerto_Asphalt++;

     	pegaValorEntrada(inputP,&resultadoGrama[0][0],freqGrass);
      error[feature+1] = feedFoward(cEntrada,
      															cOculta,
      															&cSaida,
      															inputP,
      															1.0f,
      															tamCamadaOculta,
      															&classificacao);
      backPropagation(cEntrada,cOculta,&cSaida,inputP,tamCamadaOculta,error[feature+1]);
      if (classificacao > 0.5f) acerto_Grass++;

    }

    erroEp = erroQuadMed(error);
  }

 /*******************************************************************************************/

	for (int i = 0; i < QTD_AMOSTRAS; i++)
	{
		free(resultadoGrama[i]);
		free(resultadoAsfalto[i]);
	} 
	free(resultadoGrama);
	free(resultadoAsfalto);

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

void glcm(int **imagem, double *resultado)
{
	int **glcm_Direita;
	int **glcm_Esquerda;
	int **glcm_Superior;
	int **glcm_Inferior;
	int **glcm_SuperiorDireito;
	int **glcm_SuperiorEsquerdo;
	int **glcm_InferiorDireita;
	int **glcm_InferiorEsquerda;

	glcm_Direita  = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
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
	glcm_SuperiorDireito  = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
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
	glcm_InferiorDireita  = (int **)calloc(TAMANHO_MAX_GLCM, sizeof(int *));
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
			if (j != TAMANHO_MAX_GLCM)
			{
				glcm_Direita[imagem[i][j]][imagem[i][j + 1]]++;
			}
			if (j != 0)
			{
				glcm_Esquerda[imagem[i][j]][imagem[i][j - 1]]++;
			}
			if (i != 0)
			{
        glcm_Superior[imagem[i - 1][j]][imagem[i][j]]++;
			}
			if (i != TAMANHO_MAX_GLCM)
			{
				glcm_Inferior[imagem[i + 1][j]][imagem[i][j]]++;
			}
			if ((i != 0) && (j != TAMANHO_MAX_GLCM))
			{
				glcm_SuperiorDireito[imagem[i - 1][j]][imagem[i][j + 1]]++;
			}
			if ((i != 0) && (j != 0))
			{
				glcm_SuperiorEsquerdo[imagem[i - 1][j]][imagem[i][j - 1]]++;
			}
			if ((i != TAMANHO_MAX_GLCM) && (j != TAMANHO_MAX_GLCM))
			{
				glcm_InferiorDireita[imagem[i + 1][j]][imagem[i][j + 1]]++;
			}
			if ((i != TAMANHO_MAX_GLCM) && (j != 0))
			{
				glcm_InferiorEsquerda[imagem[i + 1][j]][imagem[i][j - 1]]++;
			}
		}
	}

	glcmMetricas(glcm_Direita, resultado);
	glcmMetricas(glcm_Esquerda, resultado);
	glcmMetricas(glcm_Superior, resultado);
	glcmMetricas(glcm_Inferior, resultado);
	glcmMetricas(glcm_SuperiorDireito, resultado);
	glcmMetricas(glcm_SuperiorEsquerdo, resultado);
	glcmMetricas(glcm_InferiorDireita, resultado);
	glcmMetricas(glcm_InferiorEsquerda, resultado);

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		free(glcm_Direita[i]);
		free(glcm_Esquerda[i]);
		free(glcm_Superior[i]);
		free(glcm_Inferior[i]);
		free(glcm_SuperiorDireito[i]);
		free(glcm_SuperiorEsquerdo[i]);
		free(glcm_InferiorDireita[i]);
		free(glcm_InferiorEsquerda[i]);
	}
	free(glcm_Direita);
	free(glcm_Esquerda);
	free(glcm_Superior);
	free(glcm_Inferior);
	free(glcm_SuperiorDireito);
	free(glcm_SuperiorEsquerdo);
	free(glcm_InferiorDireita);
	free(glcm_InferiorEsquerda);
}

void glcmMetricas(int **matrizGLCM, double *glcm)
{
	double homogeneidade = 0,
			   contraste = 0,
				 energia = 0;

	for (int i = 0; i < TAMANHO_MAX_GLCM; i++)
	{
		for (int j = 0; j < TAMANHO_MAX_GLCM; j++)
		{
			contraste 		+= pow(abs(i-j),2)*matrizGLCM[i][j];
 			energia 			+= pow(matrizGLCM[i][j],2);
			homogeneidade += matrizGLCM[i][j]/(1 + abs(i-j));
		}
	}
	
	for (int i = 0; i < VETOR_GLCM; ++i)
	{
		if(glcm[i] == 0)
		{
			glcm[i]   = contraste;
			glcm[i+1] = energia;
			glcm[i+2] = homogeneidade;
			break;
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

void calculaMedia(double **matrizResultado, double *media)
{
	for (int i = 0; i < RESULTADO_VET; i++)
	{
		double aux = 0;
		for (int j = 0; j < QTD_AMOSTRAS; j++)
		{
			aux += matrizResultado[j][i];
		}
		media[i] = aux / QTD_AMOSTRAS;
	}
}

/**/

void pegaValorEntrada(double *V, double *M, int *recurrence) 
{
	int linha = rand()%50;

	while(recurrence[linha] == 0)
		linha = rand()%50;

	recurrence[linha] = 0;

	for(int i = 0; i < RESULTADO_VET; i++)
		V[i] = *(M+linha*RESULTADO_VET+i);
}


NEURONIO* alocaNeuronios(int tamCamada)
{
	NEURONIO *nr = (NEURONIO *) malloc(sizeof(NEURONIO)*tamCamada);
	return nr;
}

double* alocaPeso(int tamW)
{
	double* tempW = (double*) malloc(sizeof(double)*tamW);
	return tempW;
}

double sigmoide (double n) 
{
	double f = (1/(1-exp(-n)));
	return f;
}

double dxSigmoide (double n) 
{
 	double dx = (-1.0f)*(exp(-n))/pow((1-exp(-n)),2);
	return dx;
}

double valorMedio (NEURONIO* pn, double* inputP, int sizeV) 
{	
	double n = 0.0f;

	for (int i = 0; i < sizeV; i++) 
		n += inputP[i]*pn->w[i];

	return n += pn->b;
}

double erroQuadMed(double* erros) 
{
	double sum = 0;

	for (int i = 0; i < 50; i++) 
		sum += pow(erros[i],2);

	return sum /= 50.0f;
}

void randomizaPeso(NEURONIO *c,int tamCamada)
{
	for (int i = 0;i < tamCamada;i++)
	{
		for (int j = 0;j < c[i].tamW;j++)
			c[i].w[j] = (rand()%32000 - 16000);
    
    c[i].b = (rand()%32000 - 16000);
  }
}

double feedFoward(NEURONIO *cEntrada,
                  NEURONIO *cOculta,
                  NEURONIO *cSaida,
                  double *inputP,
                  double valorDeAtivacao,
                  int tamCamadaOculta,
                  double *classificacao)
{
  double n;
  double tempInputHidden[RESULTADO_VET];
  double *tempInputOutput;

  tempInputOutput = (double*) malloc(tamCamadaOculta*sizeof(double));
  printf("Feed Foward\n");
    
  //Entrada
  for (int i = 0;i < RESULTADO_VET;i++)
  {
    n = valorMedio(&cEntrada[i],inputP,RESULTADO_VET);
    cEntrada[i].s = sigmoide(n);
    tempInputHidden[i] = cEntrada[i].s;
  }

  //Oculta
  for (int i = 0;i < tamCamadaOculta;i++)
  {
  	n = valorMedio(&cOculta[i],tempInputHidden,RESULTADO_VET);
    cOculta[i].s = sigmoide(n);
    tempInputOutput[i] = cOculta[i].s;
  }

  //Saída
  n = valorMedio(cSaida,tempInputOutput,tamCamadaOculta);
  cSaida->s = sigmoide(n);
	
	*classificacao = cSaida->s;
  return (valorDeAtivacao - cSaida->s);
}

void backPropagation(NEURONIO *cEntrada,
										 NEURONIO *cOculta,
										 NEURONIO *cSaida,
										 double *inputP,
										 int tamCamadaOculta,
										 double erroF)
{
	double sum;

  cSaida->grad = dxSigmoide(cSaida->s) * erroF;

  for (int i = 0;i < tamCamadaOculta;i++)
  {
  	cOculta[i].grad = dxSigmoide(cOculta->s) * 
  												cSaida->grad * 
  												cSaida->w[i];
  }

  for (int i = 0;i < RESULTADO_VET;i++)
  {
  	sum = 0.0f;
    for (int j = 0; j<tamCamadaOculta; j++)
    	sum += cOculta[j].grad * cOculta[j].w[i];
		
		cEntrada[i].grad = dxSigmoide(cEntrada[i].s) * sum;
  }

  for (int i = 0;i < tamCamadaOculta;i++)
		cSaida->w[i] = cSaida->w[i]+LEARNING_RATE*cOculta[i].s*cSaida->grad;

  for (int i = 0;i < tamCamadaOculta;i++)
  {
  	for (int j = 0;j < RESULTADO_VET;j++)
  	{
  		cOculta[i].w[j] = cOculta[i].w[j]+LEARNING_RATE*cEntrada[j].s*cOculta[i].grad;
    }
  }

  for (int i = 0;i < RESULTADO_VET;i++)
  {
  	for (int j = 0;j < RESULTADO_VET;j++)
  		cEntrada[i].w[j] = cEntrada[i].w[j]+LEARNING_RATE*inputP[j]*cEntrada[i].grad;
  }

  cSaida->b = cSaida->b + LEARNING_RATE * cSaida->grad;

  for (int i = 0;i < tamCamadaOculta;i++)
		cOculta[i].b = cOculta[i].b + LEARNING_RATE * cOculta[i].grad;

  for (int i = 0;i < RESULTADO_VET;i++)
  	cEntrada[i].b = cEntrada[i].b + LEARNING_RATE * cEntrada[i].grad;
}