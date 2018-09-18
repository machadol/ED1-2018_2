#include <stdio.h>
#include <stdlib.h>
#define QTD_NOTAS 10

int  *recebe_notas(float *, int);
int  percent_aprov(int   *, int *, int *, int *, int);
void conta_notas  (int   *, int *, int *, int);

int main(int argc, char const *argv[])
{
	float NOTAS[QTD_NOTAS];
	int *pAPR, 
			*pAprova, 
			*pReprova,
			*pPercentAprova,
			*pPercentReprova;
	int aprovados     = 0, 
			reprovados    = 0, 
			perAprovados  = 0,
			perReprovados = 0,
			aprovacao;

	pAprova  = &aprovados;
	pReprova = &reprovados;
	pPercentAprova  = &perAprovados;
	pPercentReprova = &perReprovados;
	
	for (int i = 0; i < QTD_NOTAS; i++)
	{
		printf("Digite o valor da nota %d: ",i+1);
		scanf("%f",&NOTAS[i]);
	}

	pAPR = recebe_notas(NOTAS,QTD_NOTAS);
	conta_notas(pAPR,pAprova,pReprova,QTD_NOTAS);
	aprovacao = percent_aprov(pAprova,pReprova,pPercentAprova,pPercentReprova,	QTD_NOTAS);

	printf("\nQuantidade de aprovados : %d\n",*pAprova);
	printf("Quantidade de reprovados: %d\n",*pReprova);
	printf("Percentual de aprovados : %d %%\n",*pPercentAprova);
	printf("Percentual de reprovados: %d %%\n",*pPercentReprova);

	if (aprovacao)
		printf("Mais da metade da turma foi aprovada.\n");
	else
		printf("Menos da metade da turma foi aprovada.\n");

	free(pAPR);
	return 0;
}

int *recebe_notas(float *notas, int n)
{
	int *APR = (int *)malloc(n * sizeof(int));

	for (int i = 0; i < n; i++)
	{
		if (*(notas+i)>=6.0)
		{
			APR[i] = 1;
		}
		else
		{
			APR[i] = 0;
		}
	}

	return APR;
}

void conta_notas(int *apr, int *aprovados, int *reprovados, int n)
{

	for (int i = 0; i < n; i++)
	{
		if(*(apr+i)==1)
			*aprovados += 1;
		else
			*reprovados += 1;
	}
}

int percent_aprov(int *aprovados, int *reprovados, int *perAprova, int *perReprova, int n)
{
	*perAprova  = (((*aprovados)*100)/n);
	*perReprova = (((*reprovados)*100)/n);

	if (*perAprova >= 51)
		return 1;
	else 
		return 0;
}