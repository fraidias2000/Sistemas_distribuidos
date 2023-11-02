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

#define MAX_TAMANIO 3000000

long primos[1000000];
long numeroPrimos;

void mostrarPrimos(int numeroPrimos, int primos[]);
void *clienteThread(void *arguments);
void conectarServidor(char *nombreHost, int puertoHost, int opcion, int primerPrimo, int ultimoPrimo, int primos[], int *numeroPrimos);

struct conexion {
    char *nombreHost;
    int puertoHost;
    int opcion;
    int primerPrimo;
    int ultimoPrimo;
    int primos[MAX_TAMANIO];
    int numeroPrimos;
};

// Dos servidores
int main(int argc, char *argv[]) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    struct conexion conexion1, conexion2;
    int i = 0, posicionVectorPrimos = 0;
    conexion1.nombreHost = argv[1];
    conexion2.nombreHost = argv[2];

    conexion1.puertoHost = atoi(argv[3]);
    conexion2.puertoHost = atoi(argv[3]);

    conexion1.opcion = atoi(argv[4]);
    conexion2.opcion = atoi(argv[4]);

    conexion1.primerPrimo = atoi(argv[5]);
    conexion1.ultimoPrimo = atoi(argv[6]) / 2;

    conexion2.primerPrimo = atoi(argv[6]) / 2 + 1;
    conexion2.ultimoPrimo = atoi(argv[6]);

    if (pthread_create(&thread1 , NULL ,  clienteThread , &conexion1) < 0) {
            perror("\nError al crear un thread al conectar con el servidor 1");
            exit(1);
    }

    if (pthread_create(&thread2 , NULL ,  clienteThread , &conexion2) < 0) {
            perror("\nError al crear un thread al conectar con el servidor 2");
            exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    numeroPrimos = conexion1.numeroPrimos + conexion2.numeroPrimos;

    for (i = 0; i < conexion1.numeroPrimos; i++) {
            primos[i] = conexion1.primos[i];
            posicionVectorPrimos++;
    }
    for (i = 0; i < conexion2.numeroPrimos; i++) {
            primos[posicionVectorPrimos] = conexion2.primos[i];
            posicionVectorPrimos++;
    }

    printf("El número de primos en el rango %ld-%ld es %ld\n", atol(argv[5]), atol(argv[6]), numeroPrimos);
    fflush(stdout);
    exit(0);
}


void mostrarPrimos(int numeroPrimos, int primos[]) {
    for (int i = 0; i < numeroPrimos; i++) {
        printf("%ld ", primos[i]);
    }
    printf("\n");
    fflush(stdout);
}

void *clienteThread(void *arguments) {
    struct conexion *args = arguments;
    conectarServidor(args -> nombreHost, args -> puertoHost, args ->opcion, args -> primerPrimo, args -> ultimoPrimo, args -> primos, &(args->numeroPrimos));
    pthread_exit(NULL);
    return NULL;
}

void conectarServidor(char *nombreHost, int puertoHost, int opcion, int primerPrimo, int ultimoPrimo, int primos[], int *numeroPrimos) {
    struct sockaddr_in server_addr;
    struct hostent *hp;
    int s;
    int datos[3];
    clock_t tiempoInicio, tiempoFin;
    double tiempoTotal;

    datos[0] = opcion;
    datos[1] = primerPrimo; 
    datos[2] = ultimoPrimo; 

    s = socket(AF_INET, SOCK_STREAM, 0);

    // Se obtine y se rellena la dirección del servidor 
    bzero((char *)&server_addr, sizeof(server_addr));

    hp = gethostbyname(nombreHost);

    if (hp == NULL) {
        perror("\nError en la llamada gethostbyname");
        exit(0);
    }

    bcopy(hp->h_addr, (char *) & (server_addr.sin_addr), hp->h_length);
    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(puertoHost);

    // Se establece la conexión 
    if (connect(s, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("\nError en la llamada connect");
        exit(0);
    }

    tiempoInicio = clock();

    // Se envía la información
    if (write(s, &datos, sizeof(datos)) < 0) {
        perror("\nError en write");
        close(s);
        exit(1);
    }

    //Recibimos número de primos encontrados 
    if (read(s, &*numeroPrimos, sizeof(int)) < 0) {
        perror("\nError en read");
        close(s);
        exit(1);
    }
    if(opcion == 2){
        // Recibimos la lista de primos 
        char *buffer = (char *)primos;
        size_t restante = sizeof(long) * MAX_TAMANIO;

        while (restante) {
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
        printf("El número de primos en el rango %ld-%ld es %ld\n", datos[1], datos[2], *numeroPrimos);
        printf("Tiempo de cálculo del número de primos: %lf seg\n", tiempoTotal);
        fflush(stdout);
    }
    close(s);
}


