#include "primos.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define CONTAR_PRIMOS  1
#define LISTAR_PRIMOS 2  

void mostrarPrimos(primo  *resultado) {
        int i = 0;
        for (i = 0; i < resultado->numeroPrimos; i++) {
            printf("%d ", resultado->listaPrimos[i]);
        }
        printf("\n");
        fflush(stdout);
}

int main(int argc, char *argv[]) {

    
    CLIENT *sv1;
    int *resultado1;
    primo  *resultado1b;

    CLIENT *sv2;
    int *resultado2;
    primo  *resultado2b;
    
    char *nombreHost1;
    char *nombreHost2;
    int opcion;
    int min;
    int max;
    
    nombreHost1 = argv[1];
    nombreHost2 = argv[2];
    opcion = atoi(argv[3]);
    min = atoi(argv[4]);
    max = atoi(argv[5]);

    //Conectamos el primer servidor		
    sv1 = clnt_create(nombreHost1, PRIMOS, UNO, "tcp");
    if (sv1 == NULL) {
			clnt_pcreateerror (nombreHost1);
			exit (1);
		}

    //Conectamos el segundo servidor
    sv2 = clnt_create(nombreHost2, PRIMOS, DOS, "tcp");
    if (sv2 == NULL) {
			clnt_pcreateerror (nombreHost2);
			exit (1);
		}

    if(opcion == CONTAR_PRIMOS){

        resultado1 = cuentaprimos_1(min,max/2, sv1);
       
        printf("El numero de primos encontrados por el primer servidor es: %d \n", *resultado1);
        fflush(stdout);

        resultado2 = cuentaprimos_2((max/2) + 1,max, sv2);
       
        printf("El numero de primos encontrados por el segundo servidor es: %d \n", *resultado2);
        fflush(stdout);


    }else if(opcion == LISTAR_PRIMOS){

        resultado1b = encuentraprimos_1(min,max/2, sv1);
        printf("Primos devueltos por el primer servidor: ");
        mostrarPrimos(resultado1b);

        resultado2b = encuentraprimos_2((max/2) + 1,max, sv2);
        printf("Primos devueltos por el segundo servidor: ");
        mostrarPrimos(resultado2b);
        

    }
    
    clnt_destroy(sv1);
    clnt_destroy(sv2);

        exit(0);

 
}
