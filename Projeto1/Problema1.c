#include <stdio.h>
#include <stdlib.h>
#define VALOR 10

void fneuronio(double*, double*, int, int, int*);

int main(){
    double entradas[VALOR] = {0};
    double pesos[VALOR] = {0};
    double limiarT = 0.0;
    int i, a;
    int* r = &a;

    printf("ENTRADAS:\n");

    // recebe o valor das entradas

    for(i = 0; i < VALOR; i++){
        scanf("%lf", &entradas[i]);
    }

    printf("PESOS:\n");
    // recebe o valor dos pesos

    for(i = 0; i < VALOR; i++){
        scanf("%lf", &pesos[i]);
    }
    //recebe o valor da limiarT

    printf("limiarT:\n");
    scanf("%lf", &limiarT);

    fneuronio(entradas, pesos, limiarT, VALOR, r);

    if(*r == 1){
        printf("Neurônio ativado!\n");
    }else{
        printf("Neurônio inibido!\n");
    }

    return 0;
}

void fneuronio(double* entradas, double* pesos, int limiarT, int valor, int* r){
    double SOMAP = 0.0;
    int aux;

    for(aux = 0; aux < valor; aux++){
            // faz a soma ponderada.
        SOMAP += entradas[aux]*pesos[aux];
    }

    printf("Soma Ponderada: %.2f\n", SOMAP);


    if(SOMAP > limiarT){
        *r = 1;
    } else {
        *r = 0;
    }
}