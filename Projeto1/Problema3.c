#include <stdio.h>

void hanoi(int n, char inicio, char aux, char final);

int main()
{
  int n;
  printf("Digite a quantidade de discos: ");
  scanf("%d",&n);
  printf("\nSolução\n");
  hanoi(n,'A','B','C');
  return 0;
}

void hanoi(int n, char inicio, char aux, char final)
{
  if (n==1)
  {
    printf("Mova o disco 1 da torre %c para a torre %c.\n",inicio,final);
    return;
  }

  hanoi(n-1,inicio,final,aux);
  printf("Mova o disco %d da torre %c para a torre %c.\n",n,inicio,final);
  hanoi(n-1,aux,inicio,final);
}