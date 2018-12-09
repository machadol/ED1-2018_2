#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h> 

typedef struct node
{
	int value;
	struct node *left;
	struct node *right;	
}Tree;

typedef struct avlnode 
{ 
    int value; 
    struct avlnode *left; 
    struct avlnode *right; 
    int height; 
} avlTree; 

Tree *root;

Tree *loadTreeFromFile(Tree *, char []);
void showTree(Tree *);
bool isFull(Tree *);
void searchValue(Tree *, int);
int getHeight(Tree *);
Tree *removeValue(Tree *, int);
void printInOrder(Tree *);
void printPreOrder(Tree *);
void printPostOrder(Tree *);
Tree *balanceTree(Tree *, char []);

Tree *insert(Tree *, int);
int  auxShowTree(Tree *, int, int, int, char **);
int  getLevelUtil(Tree *, int, int); 
void getLevel(Tree *, int);
void getParent(Tree *, int);
void getChild(Tree *, int);
void freeTree(Tree *);
Tree *minValueNode(Tree *); 
bool isBalanced(Tree *, int *); 
    
avlTree *newNode(int);
avlTree *rightRotate(avlTree *); 
avlTree *leftRotate(avlTree *);
avlTree* createAvlTree(avlTree *, int); 
int getBalance(avlTree *);
int heightOfNode(avlTree *);

int main(int argc, char const *argv[])
{	
	int op = 0;
	char fileName[100];

	do
	{
		printf("\n\n 1. Carregar árvore do arquivo.\n");
		printf(" 2. Imprimir árvore.\n");
		printf(" 3. Verificar se é cheia.\n");
		printf(" 4. Buscar valor.\n");
		printf(" 5. Altura\n");
		printf(" 6. Remover valor.\n");
		printf(" 7. Imprimir in order.\n");
		printf(" 8. Imprimir pre order.\n");
		printf(" 9. Imprimir post order.\n");
		printf("10. Balanceamento.\n");
		printf("11. Sair\n");
		printf("Opção: ");
		scanf("%d%*c", &op);

		switch(op)
		{
			case 1:
			{
				system("clear");
				do
				{
					printf("Digite o nome do arquivo: ");
					scanf("%s", fileName);
					fflush(stdin);
					root = NULL;
					root = loadTreeFromFile(root, fileName);
				} while(root == NULL);
				break;
			}
			case 2:
				system("clear");
				showTree(root);
				break;
			case 3:
				system("clear");
				if (isFull(root)) 
					printf("Árvore é cheia.\n"); 
				else
				  printf("Árvore não é cheia\n"); 
				break;
			case 4:
			{	
				int valueToSearch;
				system("clear");
				printf("Valor a ser buscado: ");
				scanf("%d", &valueToSearch);
				searchValue(root, valueToSearch);
				break;
			}
			case 5:
				system("clear");
				printf("Altura: %d\n", getHeight(root));
				break;
			case 6:
			{
				system("clear");
				int valueToRemove;
				showTree(root);
				printf("Valor a ser removido: ");
				scanf("%d", &valueToRemove);
				root = removeValue(root, valueToRemove);
				showTree(root);
				break;
			}
			case 7:
				system("clear");
				showTree(root);
				printf("In Order: ");
				printInOrder(root);
				printf("\n\n");
				break;
			case 8:
				system("clear");
				showTree(root);
				printf("Pre Order: ");
				printPreOrder(root);
				printf("\n\n");
				break;
			case 9:
				system("clear");
				showTree(root);
				printf("Post Order: ");
				printPostOrder(root);
				printf("\n\n");
				break;
			case 10:
				system("clear");
				root = balanceTree(root, fileName);
				break;
			case 11:
				system("clear");
  			freeTree(root);
				exit(0);
		}
	} while(1);

  return 0;
}

Tree *loadTreeFromFile(Tree *r, char fileName[])
{
	FILE *fp;
  int num;
	
	fp = fopen(fileName, "r");
	if (fp == NULL){
		printf("\nArquivo não encontrado!\n\n");
		return NULL;
	}
			 	
  while((fscanf(fp,"%d",&num)) != EOF)
  {
  	r = insert(r,num);
  }

  fclose(fp);
  return r;
}

void showTree(Tree *r) 
{
	system("clear");

  int height = getHeight(r);
  int tam = ((height * 2) + 1);

  char **show = (char **)malloc( tam * sizeof(char *));
	for (int i = 0; i < tam; i++) {
		show[i] = (char *)malloc(100 * sizeof(char));
		sprintf(show[i], "%80s", " ");
	}

	auxShowTree(r, 0, 0, 0, show);

	for (int i = 0; i < tam; i++) 
	{
    int tamStr = strlen(show[i]);
    for(int j = 0; j < tamStr; j++)
    {
      printf("%c", show[i][j]);
    }
    printf("\n");
  }

  for (int i = 0; i < tam; i++)
    free(show[i]);
  free(show);
}

bool isFull(Tree *r)
{
	if (r == NULL) 
		return true; 
  
  int count = (r->left == NULL?0:1) + (r->right == NULL?0:1);

  return count != 1 && isFull(r->left) && isFull(r->right);
}

void searchValue(Tree *r, int valueToSearch)
{
	if (r == NULL)
		printf("\nNão existe o valor na árvore.\n");
	else if (r->value == valueToSearch)
	{
		if (root == r)
		{
			getLevel(root, valueToSearch);
			getParent(root, valueToSearch);
			getChild(root, valueToSearch);
		}
		if (root != r)
		{
			getLevel(root, valueToSearch);
			getParent(root, valueToSearch);
			getChild(root, valueToSearch);
		}
	}
	else if (r->value > valueToSearch)
	{
		searchValue(r->left, valueToSearch);
	}
	else if (r->value < valueToSearch)
	{
		searchValue(r->right, valueToSearch);
	}
}

int getHeight(Tree *r)
{
	if (r == NULL)
		return 0;
	else
	{
		int lHeight = getHeight(r->left);
		int rHeight = getHeight(r->right);

		if (lHeight > rHeight)
			return(lHeight + 1);
		else 
			return(rHeight +1);
	}
}

Tree *removeValue(Tree *r, int value)
{
	if (r == NULL) return r; 
  
  if (value < r->value) 
    r->left = removeValue(r->left, value); 
  else if (value > r->value) 
  	r->right = removeValue(r->right, value); 
  else
  { 
    if (r->left == NULL) 
    { 
    	Tree *temp = r->right; 
      free(r); 
      return temp; 
    } 
    else if (r->right == NULL) 
    { 
    	Tree *temp = r->left; 
      free(r); 
      return temp; 
    } 
	  Tree *temp = minValueNode(r->right); 
    r->value = temp->value; 
	  r->right = removeValue(r->right, temp->value); 
  } 
 return r; 
}

void printInOrder(Tree *r)
{
	if(r!=NULL)
  {
    printInOrder(r->left);
    printf("%d ", r->value);
    printInOrder(r->right);
  }
}

void printPreOrder(Tree *r)
{
  if(r!=NULL)
  {
  	printf("%d ", r->value);
  	printPreOrder(r->left);
    printPreOrder(r->right);
  }
}

void printPostOrder(Tree *r)
{
  if(r!=NULL)
  {
    printPostOrder(r->left);
    printPostOrder(r->right);
    printf("%d ", r->value);
  }
}

Tree *balanceTree(Tree *r, char fileName[])
{
	avlTree *avl = NULL; 	

	int height = 0;
	if(isBalanced(r, &height)) 
	{
    printf("Árvore já está balanceada."); 
  	return r;
	}
  else
  {
		FILE *fp;
  	int num;
		fp = fopen(fileName, "r");
		while((fscanf(fp,"%d",&num)) != EOF)
  	{
			avl = createAvlTree(avl, num);
	  }
		showTree((Tree *)avl);
    printf("\t\tÁrvore balanceada.\n"); 
  }

  return (Tree *)avl;
}

Tree* insert(Tree *r, int value)
{
	if(r==NULL)
  {
  	r = (Tree *) malloc(sizeof(Tree));
    r->value = value;
    r->left = NULL;
    r->right = NULL;
  }
  else if(value < r->value)
  	r->left = insert(r->left, value);
  else 
   	r->right = insert(r->right, value);
    
  return r;
}

int auxShowTree(Tree *r, int isLeft, int offset, int height, char **s) 
{
	char b[20];
  int width = 5;

  if (!r) return 0;

  sprintf(b, " %3d ", r->value);

  int left  = auxShowTree(r->left,  1, offset, height + 1, s);
  int right = auxShowTree(r->right, 0, offset + left + width, height + 1, s);

  for (int i = 0; i < width; i++)
  	s[2 * height][offset + left + i] = b[i];

  if (height && isLeft) 
  {
  	for (int i = 0; i < width + right+1; i++)
  		s[2 * height - 1][offset + left + width/2 + i] = '_';
  	s[2 * height - 1][offset + left + width + right + width/2] = '|';
  } 
  else if (height && !isLeft) 
  {
  	for (int i = 0; i < left + width; i++)
  		s[2 * height - 1][offset - width/2 + i] = '_';
  	s[2 * height - 1][offset - width/2 - 1] = '|';
  }

	return left + width + right;
}

int getLevelUtil(Tree *r, int value, int level) 
{ 
  if (r == NULL) 
    return 0; 
  
  if (r->value == value) 
    return level; 
  
  int downlevel = getLevelUtil(r->left, value, level+1); 
  if (downlevel != 0) 
  	return downlevel; 
  
  downlevel = getLevelUtil(r->right, value, level+1); 
  return downlevel; 
} 
  
void getLevel(Tree *r, int value) 
{ 

	printf("\nNível: %d\n", getLevelUtil(r, value, 1)); 
}

void getParent(Tree *r, int value)
{
	if (r->value == value)
	{
		printf("Nó raiz não possui pai.\n");
		return;
	}

	int getParentValue = 0;

	while(r != NULL)
	{
		if (value < r->value)
		{
			getParentValue = r->value;
			r = r->left;
		}
		else if (value > r->value)
		{
			getParentValue = r->value;
			r = r->right;
		}
		else
		{
			break;
		}
	}

  printf("Pai: %d\n", getParentValue); 
} 

void getChild(Tree *r, int value)
{
	if (r != NULL)
	{
		getChild(r->left, value);
		if (r->value == value)
		{
			if (r->left != NULL)
				printf("Filho esq.: %d\n",r->left->value);
			else
				printf("Sem filho a esquerda\n");

			if (r->right != NULL)
				printf("Filho dir.: %d\n",r->right->value);
			else
				printf("Sem filho a direita\n");
		}
		getChild(r->right,value);
	}
} 

void freeTree(Tree *r)
{
	if (r == NULL)
		return;
	freeTree(r->left);
	freeTree(r->right);
	free(r);
}

Tree *minValueNode(Tree *node) 
{ 
  Tree *current = node; 
  while (current->left != NULL) 
    current = current->left; 
  return current; 
}

bool isBalanced(Tree *r, int *height) 
{ 
  int lh = 0, rh = 0;   
  
  int laux = 0, raux = 0; 
      
  if(r == NULL) 
  { 
    *height = 0; 
    return 1; 
  } 
  
  laux = isBalanced(r->left, &lh); 
  raux = isBalanced(r->right,&rh); 
  
  *height = (lh>rh ? lh:rh) + 1; 
      
  if((lh - rh >= 2) || (rh - lh >= 2)) 
    return 0;     
  else 
  	return laux&&raux; 
}

avlTree* newNode(int value) 
{ 
  avlTree* node = (avlTree*)malloc(sizeof(avlTree)); 
  node->value   = value; 
  node->left   = NULL; 
  node->right  = NULL; 
  node->height = 1;  // new node is initially added at leaf 
  return(node); 
} 

avlTree *rightRotate(avlTree *y) 
{ 
  avlTree *x = y->left; 
  avlTree *T2 = x->right; 
  
  x->right = y; 
  y->left = T2; 

  int ya = heightOfNode(y->left);
  int yb = heightOfNode(y->right);
  int xa = heightOfNode(x->left);
  int xb = heightOfNode(x->right);
  y->height = ((ya > yb ? ya:yb)) + 1; 
  x->height = ((xa > xb ? xa:xb)) + 1; 

  return x; 
} 
  
avlTree *leftRotate(avlTree *x) 
{ 
  avlTree *y = x->right; 
  avlTree *T2 = y->left; 
  
  y->left = x; 
  x->right = T2; 
  
  int ya = heightOfNode(y->left);
  int yb = heightOfNode(y->right);
  int xa = heightOfNode(x->left);
  int xb = heightOfNode(x->right);
  y->height = ((ya > yb ? ya:yb)) + 1; 
  x->height = ((xa > xb ? xa:xb)) + 1; 

  return y; 
} 
  
avlTree* createAvlTree(avlTree* node, int value) 
{ 
  if (node == NULL) 
    return(newNode(value)); 
  
  if (value < node->value) 
    node->left  = createAvlTree(node->left, value); 
  else if (value > node->value) 
    node->right = createAvlTree(node->right, value); 
  else 
    return node; 

  int nodea = heightOfNode(node->left);
  int nodeb = heightOfNode(node->right);
  node->height = ((nodea > nodeb ? nodea:nodeb)) + 1; 
  
  int balance = getBalance(node); 
  
  if (balance > 1 && value < node->left->value) 
    return rightRotate(node); 

  if (balance < -1 && value > node->right->value) 
    return leftRotate(node); 

  if (balance > 1 && value > node->left->value) 
  { 
    node->left =  leftRotate(node->left); 
    return rightRotate(node); 
  } 
  
  if (balance < -1 && value < node->right->value) 
  { 
    node->right = rightRotate(node->right); 
      return leftRotate(node); 
  } 
  
  return node; 
} 

int getBalance(avlTree *N) 
{ 
  if (N == NULL) 
    return 0; 
  return heightOfNode(N->left) - heightOfNode(N->right); 
} 

int heightOfNode(avlTree *N) 
{ 
  if (N == NULL) 
    return 0; 
  return N->height; 
} 