#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define PUERTO_SERVIDOR 4400
#define MAXIMO_PRIMOS 3000000

void mostrarPrimos(int numeroPrimos, int primos[]);

int main(int argc, char *argv[]) {
  
    char *nombreHost =argv[1];
    int puertoHost = atoi(argv[2]);
    int opcion = atoi(argv[3]);
    int primerPrimo = atoi(argv[4]);
    int ultimoPrimo = atoi(argv[5]);

    struct sockaddr_in server_addr;
    struct hostent *hp;
    int s;
    int datos[3];
    clock_t tiempoInicio, tiempoFin;
    double tiempoTotal;
    static uint32_t primos[MAXIMO_PRIMOS];
    int *numeroPrimos;

    datos[0] = opcion; 
    datos[1] = primerPrimo;
    datos[2] = ultimoPrimo; 

    s = socket(AF_INET, SOCK_STREAM, 0);

    /* Se obtine y se rellena la dirección del servidor */
    bzero((char *)&server_addr, sizeof(server_addr));

    hp = gethostbyname(nombreHost);

    if (hp == NULL) {
        perror("\nError en la llamada gethostbyname");
        exit(0);
    }

    bcopy(hp->h_addr, (char *) & (server_addr.sin_addr), hp->h_length);
    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(puertoHost);

    /* Se establece la conexión */
    if (connect(s, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("\nError en la llamada connect");
        exit(0);
    }

    tiempoInicio = clock();

    /* Envía el rango de primos a calcular */
    if (write(s, &datos, sizeof(datos)) < 0) {
        perror("\nError en write");
        close(s);
        exit(1);
    }

    /* Recibe el número de primos encontrados */
    if (read(s, &*numeroPrimos, sizeof(int)) < 0) {
        perror("\nError en read");
        close(s);
        exit(1);
    }
    if(opcion == 2){
        /* Recibe los primos encontrados */
        char *buffer = (char *)primos;
        size_t restante = sizeof(int) * MAXIMO_PRIMOS;

        while (restante > 0) {
            ssize_t bytes_recibidos = read(s, buffer, restante);

            if (bytes_recibidos == -1) {
                perror("\nError en read");
                close(s);
                exit(1);
            }

            restante -= bytes_recibidos;
            buffer += bytes_recibidos;
        }
        mostrarPrimos(*numeroPrimos, primos);

        tiempoFin = clock();
        tiempoTotal = ((double)tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
        printf("El número de primos en el rango %d-%d es %d\n", datos[1], datos[2], *numeroPrimos);
        printf("Tiempo de cálculo del número de primos: %f seg\n", tiempoTotal);
        fflush(stdout);     
        close(s);
    }else{
        tiempoFin = clock();
        tiempoTotal = ((double)tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
        printf("El número de primos en el rango %d-%d es %d\n", datos[1], datos[2], *numeroPrimos);
        printf("Tiempo de cálculo del número de primos: %f seg\n", tiempoTotal);
        fflush(stdout);  

    }

}

void mostrarPrimos(int numeroPrimos, int primos[]) {
    for (int i = 0; i < numeroPrimos; i++) {
        printf("%d ", primos[i]);
    }
    printf("\n");
    fflush(stdout);


}




