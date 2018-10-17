#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Amanda
// Ana
// Bruna
// Carlos
// Fernanda
// Joao

struct CONTATO
{
	char nome[101];
 //  char telefone[11];
 //  char endereco[101];
 //  char dtNasc[11];
 //  unsigned int cep;

  struct CONTATO *anterior;
	struct CONTATO *proximo;
};

typedef struct CONTATO Pessoa;

Pessoa *insereNovo(Pessoa *, char []);
Pessoa *pElemento(Pessoa *, int);
Pessoa *criaListaVazia(Pessoa *);
void insertionSort(Pessoa *);
int tamanhoLista(Pessoa *);
void imprime(Pessoa *);
void libera(Pessoa *);



int main(int argc, char const *argv[])
{
  Pessoa *pessoas = NULL;


  pessoas = insereNovo(pessoas, "Joao");
  pessoas = insereNovo(pessoas, "Amanda");
  pessoas = insereNovo(pessoas, "Carlos");
  pessoas = insereNovo(pessoas, "Fernanda");
  pessoas = insereNovo(pessoas, "Bruna");
  pessoas = insereNovo(pessoas, "Ana");

  imprime(pessoas);
  printf("\n");
  insertionSort(pessoas);
  printf("\n");
  imprime(pessoas);

  libera(pessoas);

  return 0;
}


Pessoa *insereNovo(Pessoa *p, char n[])
{
  Pessoa *novo = (Pessoa *)malloc(sizeof(Pessoa));
    if (novo == NULL)
      exit(1);

  strcpy(novo->nome, n);
  novo->proximo = p;
  novo->anterior = NULL;

  if (p != NULL)
  {
    p->anterior = novo;
  }

  return novo;
}

int tamanhoLista(Pessoa *p)
{
  int tam = 0;

  while(p!=NULL)
  {
    p = p->proximo;

    tam ++;
  }

  return tam;
}

Pessoa *pElemento(Pessoa *p, int pos)
{
  int aux = 0;
  while(aux < pos)
  {
    p = p->proximo;
    aux++;
  }

  return p;
}


void insertionSort(Pessoa *p)
{
  char escolhido[101];
  for (int i = 1; i < tamanhoLista(p); i++)
  {
    strcpy(escolhido, (pElemento(p, i))->nome);
    int j = i - 1;

    while((j >= 0) && strcmp((pElemento(p,j)->nome), escolhido) > 0)
    {
      strcpy((pElemento(p, j+1))->nome, (pElemento(p, j))->nome);
      j--;
    }
    strcpy((pElemento(p,j+1))->nome,escolhido);
  }
}

void imprime(Pessoa *p)
{
  Pessoa *elemento;

  for (elemento = p; elemento != NULL; elemento = elemento->proximo)
  {
    printf("Nome: %s\n", elemento->nome);
  }
}
void libera(Pessoa *p)
{
  Pessoa *elemento;

  for (elemento = p; elemento != NULL; p = elemento)
  {
    elemento = elemento->proximo;
    free(p);
  }
}

