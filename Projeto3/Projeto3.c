#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  struct CONTATO  pessoa;
  struct ELEMENTO *proximo;
  struct ELEMENTO *anterior;
};

typedef struct ELEMENTO Agenda;
typedef struct CONTATO Contato;

Agenda *criaAgenda();
Agenda *insereContato(Agenda *, Contato *);
Agenda *insertionSort(Agenda *);
Agenda *pElemento(Agenda *, int);
Agenda *abreArquivo(Agenda *);
int tamanhoLista(Agenda *);
int validaDtNasc(char *);
int validaTelefone(char *);
void imprimir(Agenda *);
void liberar(Agenda *);
void buscar(Agenda *, char *);
void remover(Agenda **, char *);
void salvaContatos(Agenda *);
void menu();

int main(int argc, char const *argv[])
{
  Agenda *agenda;
  agenda = criaAgenda();
  int op = 0;

  agenda = abreArquivo(agenda);

  do
  {
    printf("\n\n");
    menu();
    scanf("%d%*c", &op);
    switch(op)
    {
      case 1: // Inserir
        {
          char nome[102];
          char telefone[12];
          char endereco[102];
          char dtNasc[12];
          unsigned int cep;
          int valida = 0;

          Contato *contatoPessoa = (Contato *)malloc(sizeof(Contato));

          system("clear");
          printf("Informe o nome: ");
          fgets(nome, sizeof(nome), stdin);
          nome[strlen(nome) - 1] = '\0';

          do
          {
            printf("Informe o telefone no formato XXXXX-XXXX: ");
            fgets(telefone, sizeof(telefone), stdin);
            telefone[strlen(telefone) - 1] = '\0';
            valida = validaTelefone(telefone);
            
            if (!valida)
              printf("\nFormato inválido, tente novamente!\n\n"); 
          }
          while (!valida);

          printf("Informe o endereço: ");
          fgets(endereco, sizeof(endereco), stdin);
          endereco[strlen(endereco) - 1] = '\0';
          
          printf("Informe o CEP: ");
          scanf("%u%*c", &contatoPessoa->cep);

          do
          {
            printf("Informe a data de nascimento no formato dd/mm/aaaa: ");
            fgets(dtNasc, sizeof(dtNasc), stdin);
            dtNasc[strlen(dtNasc) - 1] = '\0';
            valida = validaDtNasc(dtNasc);
  
            if (!valida)
              printf("\nFormato inválido, tente novamente!\n\n");
          }
          while (!valida);

          strcpy(contatoPessoa->nome, nome);
          strcpy(contatoPessoa->dtNasc, dtNasc);
          strcpy(contatoPessoa->telefone, telefone);
          strcpy(contatoPessoa->endereco, endereco);

          agenda = insertionSort(insereContato(agenda, contatoPessoa));
          free(contatoPessoa);
        }      
        break;
      case 2: // Remover
        {
          char removeNome[101];

          printf("\nRemover onde tenha: ");
          fgets(removeNome, sizeof(removeNome), stdin);
          removeNome[strlen(removeNome) - 1] = '\0';
          system("clear");
          remover(&agenda, removeNome);
        }
        break;
      case 3: // Buscar
        {
          char buscaNome[101];

          printf("\nBuscar onde tenha: ");
          fgets(buscaNome, sizeof(buscaNome), stdin);
          buscaNome[strlen(buscaNome) - 1] = '\0';
          system("clear");
          buscar(agenda, buscaNome);
        }
        break;
      case 4: //Imprimir
        system("clear");
        imprimir(agenda);
        break;
      case 5: // Sair
        system("clear");
        salvaContatos(agenda);
        liberar(agenda);
        exit(0);
    }
  } while(1);

  return 0;
}

Agenda *criaAgenda()
{
  Agenda *ag = (Agenda *)malloc(sizeof(Agenda));
  if (ag != NULL)
    ag = NULL;
  return ag;
}

Agenda *insereContato(Agenda *ag, Contato *contatoPessoa)
{
  Agenda *novo = (Agenda *)malloc(sizeof(Agenda));
    if (novo == NULL)
      exit(1);

  novo->pessoa = *contatoPessoa;
  novo->proximo = ag;
  novo->anterior = NULL;

  if (ag != NULL)
  {
    ag->anterior = novo;
  }

  return novo;
}

Agenda *insertionSort(Agenda *ag)
{
  char nomeEscolhido[101];
  char telefoneEscolhido[11];
  char enderecoEscolhido[101];
  char dtNascEscolhido[11];
  unsigned int cepEscolhido;

  for (int i = 1; i < tamanhoLista(ag); i++)
  {
    strcpy(nomeEscolhido, (pElemento(ag, i))->pessoa.nome);
    strcpy(telefoneEscolhido, (pElemento(ag, i))->pessoa.telefone);
    strcpy(enderecoEscolhido, (pElemento(ag, i))->pessoa.endereco);
    strcpy(dtNascEscolhido, (pElemento(ag, i))->pessoa.dtNasc);
    cepEscolhido = (pElemento(ag,i))->pessoa.cep;
    
    int j = i - 1;

    while((j >= 0) && strcmp((pElemento(ag,j)->pessoa.nome), nomeEscolhido) > 0)
    {
      strcpy((pElemento(ag, j+1))->pessoa.nome, (pElemento(ag, j))->pessoa.nome);      
      strcpy((pElemento(ag, j+1))->pessoa.telefone, (pElemento(ag, j))->pessoa.telefone);
      strcpy((pElemento(ag, j+1))->pessoa.endereco, (pElemento(ag, j))->pessoa.endereco);
      strcpy((pElemento(ag, j+1))->pessoa.dtNasc, (pElemento(ag, j))->pessoa.dtNasc);
      (pElemento(ag, j+1))->pessoa.cep = (pElemento(ag, j))->pessoa.cep;

      j--;
    }
    strcpy((pElemento(ag,j+1))->pessoa.nome,nomeEscolhido);
    strcpy((pElemento(ag,j+1))->pessoa.telefone, telefoneEscolhido);
    strcpy((pElemento(ag,j+1))->pessoa.endereco, enderecoEscolhido);
    strcpy((pElemento(ag,j+1))->pessoa.dtNasc,   dtNascEscolhido);
    (pElemento(ag,j+1))->pessoa.cep = cepEscolhido;
  }
  return ag;
}

Agenda *pElemento(Agenda *ag, int pos)
{
  int aux = 0;
  while(aux < pos)
  {
    ag = ag->proximo;
    aux++;
  }
  return ag;
}

Agenda *abreArquivo(Agenda *ag)
{
  system("clear");
  FILE *agendaContato = fopen("contatos.txt", "r");
  if (agendaContato == NULL)
  {
    printf("Criando agenda!");
    agendaContato = fopen("contatos.txt", "w");
    return 0;
  }
  else
  {
    printf("Agenda aberta!");
  }

  int linhas = 0, aux = 0;
  int nome = 0, telefone = 1, endereco = 2, cep = 3, dtNasc = 4, cifrao = 5;
  char **conteudoLinha;
  char lerLinha[101];
  char ch;
  Contato *contatoPessoa;

  while((ch = getc(agendaContato)) != EOF)
  {
    if (ch == '\n')
      linhas++;
  }
  linhas++;

  conteudoLinha = malloc(linhas*sizeof(char *));
    for (int i = 0; i < linhas; i++)
      conteudoLinha[i] = malloc(101*sizeof(char));

  if (conteudoLinha == NULL)
  {
    printf("Fail\n");
    exit(1);
  }

  rewind(agendaContato);

  while (fgets(lerLinha, sizeof(lerLinha), agendaContato) != NULL)
  {
    lerLinha[strlen(lerLinha) - 1] = '\0';
    strcpy(conteudoLinha[aux],lerLinha);
    aux++;    
  }

  for (int i = 0; i < linhas; i++)
  {
    if (i == nome)
    {
      contatoPessoa = (Contato *)malloc(sizeof(Contato));
      strcpy(contatoPessoa->nome, conteudoLinha[i]);
      nome+=6;
    }
    if (i == telefone)
    {
      strcpy(contatoPessoa->telefone, conteudoLinha[i]);
      telefone+=6;
    }
    if (i == endereco)
    {
      strcpy(contatoPessoa->endereco, conteudoLinha[i]);
      endereco+=6;
    }
    if (i == cep)
    {
      unsigned int cepNum = atoi(conteudoLinha[i]);
      contatoPessoa->cep =  cepNum;
      cep+=6;
    }
    if (i == dtNasc)
    {
      strcpy(contatoPessoa->dtNasc, conteudoLinha[i]);
      dtNasc+=6;
    }
    if (i == cifrao)
    {
      ag = insertionSort(insereContato(ag, contatoPessoa));
      free(contatoPessoa);
      cifrao+=6;
    }
  }
  
  for (int i = 0; i < linhas; i++) free(conteudoLinha[i]);
  free(conteudoLinha);
  
  fclose(agendaContato);
  return ag;
}

int tamanhoLista(Agenda *ag)
{
  int tam = 0;

  while(ag!=NULL)
  {
    ag = ag->proximo;

    tam ++;
  }

  return tam;
}

int validaDtNasc(char *dtNasc)
{
  int tam = strlen(dtNasc);

  if (tam != 10)
    return 0;

  for (int i = 0; i < tam; i++)
  {
    if (dtNasc[i] > '0' || dtNasc[i] < '9')
    {
      if (i == 2 && dtNasc[i] != '/')
        return 0;
      if (i == 5 && dtNasc[i] != '/')
        return 0;
    }
  }
  return 1;
}

int validaTelefone(char *telefone)
{
  int tam = strlen(telefone);

  if (tam != 10)
    return 0;

  for (int i = 0; i < tam - 1; i++)
  {
    if (telefone[i] > '0' || telefone[i] < '9')
    {
      if (i == 5 && telefone[i] != '-')
        return 0;
    }
  }
  return 1;
}

void imprimir(Agenda *ag)
{
  Agenda *no = ag;

  if (no == NULL)
    printf("Agenda vazia!");
  else
    for (no = ag; no != NULL; no = no->proximo)
    {
      printf("Nome.................. %s\n", no->pessoa.nome);
      printf("Telefone.............. %s\n", no->pessoa.telefone);
      printf("Endereço.............. %s\n", no->pessoa.endereco);
      printf("CEP................... %u\n", no->pessoa.cep);
      printf("Data nascimento....... %s\n\n", no->pessoa.dtNasc);
    }
}

void liberar(Agenda *ag)
{
  Agenda *no;
  for (no = ag; no != NULL; ag = no)
  {
    no = no->proximo;
    free(ag);
  }
}

void buscar(Agenda *ag , char *buscaNome)
{ 
  Agenda *no = ag;
  int aux = 0;

  for (no = ag; no != NULL; no = no->proximo)
  {
    if (strstr(no->pessoa.nome, buscaNome)!=NULL)
    {
      printf("Nome: %s\n", no->pessoa.nome);
      printf("Data nascimento: %s\n", no->pessoa.dtNasc);
      printf("Telefone: %s\n", no->pessoa.telefone);
      printf("Endereço: %s\n", no->pessoa.endereco);
      printf("CEP: %d\n\n", no->pessoa.cep);
    }
    else
      aux++;
  }
  if (aux == tamanhoLista(ag))
  {
    printf("Contato não encontrado!");
  }
}

void remover(Agenda **ag , char *removeNome)
{ 
  if (*ag == NULL)
  {
    printf("Agenda vazia!\n");
  }
  else
  {
    Agenda *temp = *ag, *aux  = *ag;

    while(temp)
    {
      if (strstr(temp->pessoa.nome, removeNome) != NULL)
      {
        if (temp == *ag)
        {
          aux  = temp;
          *ag  = (*ag)->proximo;
          temp = *ag;
          if(aux->proximo != NULL)
            aux->proximo->anterior = NULL;
          aux->proximo = NULL;
          free(aux);
          aux = NULL;
          printf("Contato removido!\n");
        }
        else
        {
          aux->proximo  = temp->proximo;
          temp->proximo  = NULL;
          temp->anterior = NULL;

          free(temp);
          temp = aux->proximo;
          printf("Contato removido!\n");
        }
      }
      else
      {
        aux = temp; 
        temp = temp->proximo;
      }
    }
  }
}

void salvaContatos(Agenda *ag)
{
  Agenda *no;

  FILE *agendaContato = fopen("contatos.txt", "w");

  for (no = ag; no != NULL; no = no->proximo)
  {
    fprintf(agendaContato, "%s\n", no->pessoa.nome);
    fprintf(agendaContato, "%s\n", no->pessoa.telefone);
    fprintf(agendaContato, "%s\n", no->pessoa.endereco);
    fprintf(agendaContato, "%u\n", no->pessoa.cep);
    fprintf(agendaContato, "%s\n$\n", no->pessoa.dtNasc);
  }
  printf("Contatos salvos em contatos.txt!\n");
  fclose(agendaContato);
}

void menu()
{
  printf("************************\n");
  printf("*\t AGENDA        *\n");
  printf("************************\n");
  printf("*  Opções:             *");
  printf("\n*  1. Inserir.         *" );
  printf("\n*  2. Remover.         *" );
  printf("\n*  3. Buscar.          *" );
  printf("\n*  4. Imprimir.        *" );
  printf("\n*  5. Sair.            *" );
  printf("\n************************");
  printf("\nDigite a opção desejada: ");
}