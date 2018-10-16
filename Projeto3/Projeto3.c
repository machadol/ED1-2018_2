#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CONTATO
{
	char nome[101];
	char telefone[11];
	char endereco[101];
	unsigned int cep;
	char dtNasc[11];

	struct CONTATO *p;
	struct CONTATO *a;
};

typedef struct CONTATO Contato;

int main(int argc, char const *argv[])
{
	int op = 0;

  do
  {
    system("clear");
    printf("\tAgenda");
    printf(" \n 1. Inserir novo contato." );
    printf(" \n 2. Remover contato." );
    printf(" \n 3. Buscar contato." );
    printf(" \n 4. Visualizar agenda.");
    printf(" \n 5. Sair.");
    printf("\nDigite a opção desejada: ");
    scanf("%d%*c", &op);
    switch(op)
    {
      case 1:
        break;
      case 2:
        break;
      case 3:
      	break;
      case 4:
      	break;
      case 5:
        exit(0);
    }
  } while(1);
	
	return 0;
}
