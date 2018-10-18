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
  char telefone[11];
  char endereco[101];
  char dtNasc[11];
  unsigned int cep;
};

struct ELEMENTO
{
  struct CONTATO pessoa;
  struct ELEMENTO *proximo;
  struct ELEMENTO *anterior;
};

typedef struct ELEMENTO Elemento;

Elemento *insereNovo(Elemento *);
Elemento *pElemento(Elemento *, int);
Elemento *criaListaVazia(Elemento *);
Elemento *insertionSort(Elemento *);

int tamanhoLista(Elemento *);
void imprime(Elemento *);
void libera(Elemento *);
void busca(Elemento *);

int main(int argc, char const *argv[])
{
  Elemento *p = NULL;

  p = insertionSort(insereNovo(p));
  imprime(p);
  p = insertionSort(insereNovo(p));
  imprime(p);
  p = insertionSort(insereNovo(p));
  imprime(p);
  p = insertionSort(insereNovo(p));
  imprime(p);
  // busca(p); 
  printf("\n");

  libera(p);
  return 0;
}

Elemento *insereNovo(Elemento *p)
{
  Elemento *novo = (Elemento *)malloc(sizeof(Elemento));
    if (novo == NULL)
      exit(1);

  char nome[101];
  char telefone[11];
  char endereco[101];
  char dtNasc[11];
  unsigned int cep;

  // system("clear");
  printf("Informe o nome: ");
  fgets(nome, sizeof(nome), stdin);

  printf("Informe a data de nascimento: ");
  fgets(dtNasc, sizeof(dtNasc), stdin);
  
  printf("Informe o telefone: ");
  fgets(telefone, sizeof(telefone), stdin);

  printf("Informe o endereço: ");
  fgets(endereco, sizeof(endereco), stdin);
  
  printf("Informe o CEP: ");
  scanf("%d%*c", &novo->pessoa.cep);

  if (nome[strlen(nome) - 1] == '\n')
      nome[strlen(nome) - 1] = 0;
  if (dtNasc[strlen(dtNasc) - 1] == '\n')
      dtNasc[strlen(dtNasc) - 1] = 0;
  if (telefone[strlen(telefone) - 1] == '\n')
      telefone[strlen(telefone) - 1] = 0;
  if (endereco[strlen(endereco) - 1] == '\n')
      endereco[strlen(endereco) - 1] = 0;

  strcpy(novo->pessoa.nome, nome);
  strcpy(novo->pessoa.dtNasc, dtNasc);
  strcpy(novo->pessoa.telefone, telefone);
  strcpy(novo->pessoa.endereco, endereco);
  

  novo->proximo = p;
  novo->anterior = NULL;

  if (p != NULL)
  {
    p->anterior = novo;
  }

  return novo;
}

int tamanhoLista(Elemento *p)
{
  int tam = 0;

  while(p!=NULL)
  {
    p = p->proximo;

    tam ++;
  }

  return tam;
}

Elemento *pElemento(Elemento *p, int pos)
{
  int aux = 0;
  while(aux < pos)
  {
    p = p->proximo;
    aux++;
  }

  return p;
}

// Elemento *insertionSort(Elemento *p)
// {
//   char escolhido[101];
//   for (int i = 1; i < tamanhoLista(p); i++)
//   {
//     strcpy(escolhido, (pElemento(p, i))->pessoa.nome);
//     int j = i - 1;

//     while((j >= 0) && strcmp((pElemento(p,j)->pessoa.nome), escolhido) > 0)
//     {
//       strcpy((pElemento(p, j+1))->pessoa.nome, (pElemento(p, j))->pessoa.nome);
//       j--;
//     }
//     strcpy((pElemento(p,j+1))->pessoa.nome,escolhido);
//   }

//   return p;
// }

Elemento *insertionSort(Elemento *p)
{
  char nomeEscolhido[101];
  char telefoneEscolhido[11];
  char enderecoEscolhido[101];
  char dtNascEscolhido[11];
  unsigned int cepEscolhido;

  for (int i = 1; i < tamanhoLista(p); i++)
  {
    strcpy(nomeEscolhido, (pElemento(p, i))->pessoa.nome);
    strcpy(telefoneEscolhido, (pElemento(p, i))->pessoa.telefone);
    strcpy(enderecoEscolhido, (pElemento(p, i))->pessoa.endereco);
    strcpy(dtNascEscolhido, (pElemento(p, i))->pessoa.dtNasc);
    cepEscolhido = (pElemento(p,i))->pessoa.cep;
    
    int j = i - 1;

    while((j >= 0) && strcmp((pElemento(p,j)->pessoa.nome), nomeEscolhido) > 0)
    {
      strcpy((pElemento(p, j+1))->pessoa.nome, (pElemento(p, j))->pessoa.nome);      
      strcpy((pElemento(p, j+1))->pessoa.telefone, (pElemento(p, j))->pessoa.telefone);
      strcpy((pElemento(p, j+1))->pessoa.endereco, (pElemento(p, j))->pessoa.endereco);
      strcpy((pElemento(p, j+1))->pessoa.dtNasc, (pElemento(p, j))->pessoa.dtNasc);
      (pElemento(p, j+1))->pessoa.cep = (pElemento(p, j))->pessoa.cep;

      j--;
    }
    strcpy((pElemento(p,j+1))->pessoa.nome,nomeEscolhido);
    strcpy((pElemento(p,j+1))->pessoa.telefone, telefoneEscolhido);
    strcpy((pElemento(p,j+1))->pessoa.endereco, enderecoEscolhido);
    strcpy((pElemento(p,j+1))->pessoa.dtNasc,   dtNascEscolhido);
    (pElemento(p,j+1))->pessoa.cep = cepEscolhido;
  }
  return p;
}

void imprime(Elemento *p)
{
  Elemento *elemento;

  // system("clear");
  for (elemento = p; elemento != NULL; elemento = elemento->proximo)
  {
    printf("Nome: %s\n", elemento->pessoa.nome);
    printf("Data nascimento: %s\n", elemento->pessoa.dtNasc);
    printf("Telefone: %s\n", elemento->pessoa.telefone);
    printf("Endereço: %s\n", elemento->pessoa.endereco);
    printf("CEP: %d\n\n", elemento->pessoa.cep);
  }
}

void libera(Elemento *p)
{
  Elemento *elemento;

  for (elemento = p; elemento != NULL; p = elemento)
  {
    elemento = elemento->proximo;
    free(p);
  }
}

void busca(Elemento *p)
{ 
  Elemento *elemento;

  char buscaNome[101];
  char nome[101];

  printf("Buscar por: ");
  fgets(nome, sizeof(nome), stdin);
  if (nome[strlen(nome) - 1] == '\n')
    nome[strlen(nome) - 1] = 0;


  for (elemento = p; elemento != NULL; elemento = elemento->proximo)
  {
    if (strstr(elemento->pessoa.nome, buscaNome)!=NULL)
    {
      printf("Nome: %s\n", elemento->pessoa.nome);
      printf("Data nascimento: %s\n", elemento->pessoa.dtNasc);
      printf("Telefone: %s\n", elemento->pessoa.telefone);
      printf("Endereço: %s\n", elemento->pessoa.endereco);
      printf("CEP: %d\n\n", elemento->pessoa.cep);
    }
  }

}