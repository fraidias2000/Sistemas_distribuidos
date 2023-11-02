#include "primos.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

void mostrarPrimos(int numeroPrimos, primo  *result_2) {
    printf("Primos devueltos por el servidor: ");
    int i = 0;
    for (i = 0; i < numeroPrimos; i++) {
         printf("%ld ", result_2->primos[i]);
    }
    printf("\n");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    char *nombreHost = argv[1];
    int opcion = atoi(argv[2]);
    int primerPrimo = atoi(argv[3]); 
    int ultimoPrimo = atoi(argv[4]);
    CLIENT *sv;
    primo  *result;
    int min, max;
    min = primerPrimo;
    max = ultimoPrimo;
    clock_t inicio, final;
    double segundos;

    sv = clnt_create(nombreHost, PRIMOS, UNO, "tcp");

    if (sv == NULL) {
        clnt_pcreateerror(nombreHost);
        exit(1);
    }
    inicio = clock();
    if(opcion == 1){
        result->numeroPrimos = cuenta_primos_1(min,max, sv);
        if (result == NULL) {
            clnt_perror(sv, "llamada fallida");
            exit(1);
        }
        printf("La cantidad de primos encontrados entre %d y %d es: %d",min, max, result->numeroPrimos);

    }else{
        result = encuentra_primos_1(min,max, sv);

        if (result == NULL) {
            clnt_perror(sv, "llamada fallida");
            exit(1);
        }
        mostrarPrimos(result->numeroPrimos, result);

    }
    final = clock();
    segundos = (double)(final - inicio) / CLOCKS_PER_SEC;
    printf("Tiempo: %ld segundos\n", segundos);
    
    clnt_destroy(sv);


    exit(0);
}
