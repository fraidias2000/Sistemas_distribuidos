#include "primos.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#define MAXIMO_PRIMOS_A_MOSTRAR 8000000

void mostrarPrimos(primo  *result_2) {
        printf("Primos devueltos por el servidor: ");
        int i = 0;

        for (i = 0; i < result_2->numeroPrimos; i++) {
            printf("%ld ", result_2->primos[i]);
        }
        printf("\n");
        fflush(stdout);
}


struct conexion {
    char *nombreHost;
    int primerNumero;
    int ultimoNumero;
};


int main(int argc, char *argv[]) { {
    struct conexion c1, c2, c3, c4;
    CLIENT *sv1, *sv2, *sv3, *sv4;
    primo  *result_1, *result_2, *result_3,*result_4;
    struct timeval tiempo_inicio, tiempo_final;
    int tiempoTotal;
    int opcion;
    int primerNumero = atoi(argv[3]);
    int ultimoNumero = atoi(argv[4]);

    c1.nombreHost = argv[1];
    c2.nombreHost = argv[1];
    c3.nombreHost = argv[1];
    c4.nombreHost = argv[1];
    
    opcion = atoi(argv[2]);

    c1.primerNumero = primerNumero;
    c1.ultimoNumero = ultimoNumero / 4;

    c2.primerNumero = ultimoNumero / 4 + 1;
    c2.ultimoNumero = (ultimoNumero / 4) * 2;

    c3.primerNumero = (ultimoNumero/ 4) * 2 + 1;
    c3.ultimoNumero = (ultimoNumero / 4) * 3;

    c4.primerNumero = (ultimoNumero / 4) * 3 + 1;
    c4.ultimoNumero = ultimoNumero;


    //Llamada 1 servidor
    sv1 = clnt_create(nombreHost, PRIMOS, UNO, "tcp"); //Conectamos el primer servidor

    if (sv1 == NULL) {
        clnt_pcreateerror("Error al conectar el 1º servidor");
        exit(1);
    }

    //Llamada 2 servidor
    sv2 = clnt_create(nombreHost, PRIMOS, DOS, "tcp"); //Conectamos el segundo servidor
    if (sv2 == NULL) {
        clnt_pcreateerror("Error al conectar el 2º servidor");
        exit(1);
    }

    //Llamada 3 servidor
    sv3 = clnt_create(nombreHost, PRIMOS, TRES, "tcp"); //Conectamos el tercer servidor
    if (sv3 == NULL) {
        clnt_pcreateerror("Error al conectar el 3º servidor");
        exit(1);
    }


    //Llamada 4 servidor
    sv4 = clnt_create(nombreHost, PRIMOS, CUATRO, "tcp"); //Conectamos el cuarto servidor
    if (sv4 == NULL) {
        clnt_pcreateerror("Error al conectar el 4º servidor");
        exit(1);
    }

    if(opcion == 1){
        //1º SERVIDOR
        result_1->numeroPrimos = cuenta_primos_1(c1.primerNumero,c1.ultimoNumero, sv1);

        if (result_1 == NULL) {
            clnt_perror(sv1, "Primera llamada fallida");
        }
        printf("El número de primos del 1 servidor en el rango %ld-%ld es %ld\n", c1.primerNumero, c1.ultimoNumero, result_1->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_1);
        clnt_destroy(sv1);

        //2º SERVIDOR
        result_2->numeroPrimos  = cuenta_primos_1(c2.primerNumero,c2.ultimoNumero, sv2);
        if (result_2 == NULL) {
            clnt_perror(sv2, "Segunda llamada fallida");
        }
        printf("El número de primos del 2 servidor en el rango %ld-%ld es %ld\n", c2.primerNumero, c2.ultimoNumero, result_2->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_2);
        clnt_destroy(sv2);

        //3º SERVIDOR
        result_3->numeroPrimos  = cuenta_primos_1(c3.primerNumero,c3.ultimoNumero, sv3);
        if (result_3 == NULL) {
            clnt_perror(sv3, "Segunda llamada fallida");
        }
        printf("El número de primos del 3 servidor en el rango %ld-%ld es %ld\n", c3.primerNumero, c3.ultimoNumero, result_3->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_3);
        clnt_destroy(sv3);

        //4º SERVIDOR
        result_4->numeroPrimos  = cuenta_primos_1(c4.primerNumero,c4.ultimoNumero, sv4);
        if (result_4 == NULL) {
            clnt_perror(sv4, "Segunda llamada fallida");
        }
        printf("El número de primos del 4 servidor en el rango %ld-%ld es %ld\n", c4.primerNumero, c4.ultimoNumero, result_4->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_4);
        clnt_destroy(sv4);
    }else{ //LISTAR PRIMOS
        //1º SERVIDOR
        result_1 = encuentra_primos_1(c1.primerNumero,c1.ultimoNumero, sv1);

        if (result_1 == NULL) {
            clnt_perror(sv1, "Primera llamada fallida");
        }
        printf("El número de primos del 1 servidor en el rango %ld-%ld es %ld\n", c1.primerNumero, c1.ultimoNumero, result_1->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_1);
        clnt_destroy(sv1);

        //2º SERVIDOR
        result_2 = encuentra_primos_1(c2.primerNumero,c2.ultimoNumero, sv2);
        if (result_2 == NULL) {
            clnt_perror(sv2, "Segunda llamada fallida");
        }
        printf("El número de primos del 2 servidor en el rango %ld-%ld es %ld\n", c2.primerNumero, c2.ultimoNumero, result_2->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_2);
        clnt_destroy(sv2);

        //3º SERVIDOR
        result_3 = encuentra_primos_1(c3.primerNumero,c3.ultimoNumero, sv3);
        if (result_3 == NULL) {
            clnt_perror(sv3, "Segunda llamada fallida");
        }
        printf("El número de primos del 3 servidor en el rango %ld-%ld es %ld\n", c3.primerNumero, c3.ultimoNumero, result_3->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_3);
        clnt_destroy(sv3);

        //4º SERVIDOR
        result_4 = encuentra_primos_1(c4.primerNumero,c4.ultimoNumero, sv4);
        if (result_4 == NULL) {
            clnt_perror(sv4, "Segunda llamada fallida");
        }
        printf("El número de primos del 4 servidor en el rango %ld-%ld es %ld\n", c4.primerNumero, c4.ultimoNumero, result_4->numeroPrimos);
        fflush(stdout);
        mostrarPrimos(result_4);
        clnt_destroy(sv4);
    }
}

int main(int argc, char *argv[]) {
    conectarServidor(argv[1], atoi(argv[2]), atoi(argv[3]));
    exit(0);
}
