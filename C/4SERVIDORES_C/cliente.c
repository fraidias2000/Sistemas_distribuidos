#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define PUERTO_SERVIDOR 4400
#define MAXIMO_TAMANIO 3000000

void mostrarPrimos(int numeroPrimos, int primos[]) ;
void conectarServidor(char *nombreHost, int puertoHost, int opcion,int primerPrimo, int ultimoPrimo, int primos[], int *numeroPrimos);
void *conectarThread(void *arguments);

struct conexion {
    char *nombreHost;
    int puertoHost;
    int opcion;
    int primerPrimo;
    int ultimoPrimo;
    int primos[MAXIMO_TAMANIO];
    int numeroPrimos;
};

int main(int argc, char *argv[]) {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;

    struct conexion conexion_1;
    struct conexion conexion_2;
    struct conexion conexion_3;
    struct conexion conexion_4;

    int i = 0;
    int posicionVectorPrimos = 0;

     // Cuatro servidores
    conexion_1.nombreHost = argv[1];
    conexion_2.nombreHost = argv[1];
    conexion_3.nombreHost = argv[1];
    conexion_4.nombreHost = argv[1];

    conexion_1.puertoHost = atoi(argv[2]);
    conexion_2.puertoHost = atoi(argv[3]);
    conexion_3.puertoHost = atoi(argv[4]);
    conexion_4.puertoHost = atoi(argv[5]);

    conexion_1.opcion = atoi(argv[6]);
    conexion_2.opcion = atoi(argv[6]);
    conexion_3.opcion = atoi(argv[6]);
    conexion_4.opcion = atoi(argv[6]);

    conexion_1.primerPrimo = atoi(argv[7]);
    conexion_1.ultimoPrimo = atoi(argv[8]) / 4;

    conexion_2.primerPrimo = atoi(argv[8]) / 4 + 1;
    conexion_2.ultimoPrimo = (atoi(argv[8]) / 4) * 2;

    conexion_3.primerPrimo = (atoi(argv[8]) / 4) * 2 + 1;
    conexion_3.ultimoPrimo = (atoi(argv[8]) / 4) * 3;

    conexion_4.primerPrimo = (atoi(argv[8]) / 4) * 3 + 1;
    conexion_4.ultimoPrimo = atoi(argv[8]);

    if (pthread_create(&thread1 , NULL ,  conectarThread , &conexion_1) < 0) {
        perror("\nError en el thread 1");
        exit(1);
    }

    if (pthread_create(&thread2 , NULL ,  conectarThread , &conexion_2) < 0) {
        perror("\nError en el thread 2");
        exit(1);
    }

    if (pthread_create(&thread3 , NULL ,  conectarThread , &conexion_3) < 0) {
        perror("\nError en el thread 3");
        exit(1);
    }

    if (pthread_create(&thread4 , NULL ,  conectarThread , &conexion_4) < 0) {
        perror("\nError en el thread 4");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    numeroPrimos = conexion_1.numeroPrimos + conexion_2.numeroPrimos + conexion_3.numeroPrimos + conexion_4.numeroPrimos;

    for (i = 0; i < conexion_1.numeroPrimos; i++) {
        primos[i] = conexion_1.primos[i];
        posicionVectorPrimos++;
    }

    for (i = 0; i < conexion_2.numeroPrimos; i++) {
        primos[posicionVectorPrimos] = conexion_2.primos[i];
        posicionVectorPrimos++;
    }
    for (i = 0; i < conexion_3.numeroPrimos; i++) {
        primos[posicionVectorPrimos] = conexion_3.primos[i];
        posicionVectorPrimos++;
    }

    for (i = 0; i < conexion_4.numeroPrimos; i++) {
        primos[posicionVectorPrimos] = conexion_4.primos[i];
        posicionVectorPrimos++;
    }

    printf("El número de primos en el rango %ld-%ld es %ld\n", atoi(argv[7]), atoi(argv[8]), numeroPrimos);
    fflush(stdout);
    exit(0);
}

void mostrarPrimos(int numeroPrimos, int primos[]) {
        printf("Primos devueltos por el servidor: ");
        int i = 0;
        for (i = 0; i < numeroPrimos; i++) {
            printf("%ld ", primos[i]);
        }
        printf("\n");
        fflush(stdout);
}
void *conectarThread(void *arguments) {
    struct conexion *args = arguments;
    conectarServidor(args -> nombreHost, args -> puertoHost,args ->opcion, args -> primerPrimo, args -> ultimoPrimo, args -> primos, &(args->numeroPrimos));
    pthread_exit(NULL);
    return NULL;
}

void conectarServidor(char *nombreHost, int puertoHost, int opcion,int primerPrimo, int ultimoPrimo, int primos[], int *numeroPrimos) {
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

    //Se establece la conexión 
    if (connect(s, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("\nError en la llamada connect");
        exit(0);
    }

    tiempoInicio = clock();

    //Envía el rango de primos a calcular 
    if (write(s, &datos, sizeof(datos)) < 0) {
        perror("\nError en write");
        close(s);
        exit(1);
    }

    // Recibe el número de primos encontrados 
    if (read(s, &*numeroPrimos, sizeof(int)) < 0) {
        perror("\nError en read");
        close(s);
        exit(1);
    }
    if(opcion == 2){
        // Recibe los primos encontrados
        char *buffer = (char *)primos;
        size_t restante = sizeof(int) * MAXIMO_TAMANIO;
        while (restante) {
            ssize_t bytes_recibidos = read(s, buffer, restante);
            printf("%d",bytes_recibidos);
            if (bytes_recibidos == -1) {
                perror("\nError en read");
                close(s);
                exit(1);
            }
            restante -= bytes_recibidos;
            buffer += bytes_recibidos;
        }
        tiempoFin = clock();
        tiempoTotal = ((double)tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
        printf("El número de primos en el rango %ld-%ld es %ld\n", datos[1], datos[2], *numeroPrimos);
        printf("Tiempo de cálculo del número de primos: %f seg\n", tiempoTotal);
        fflush(stdout);
        mostrarPrimos(*numeroPrimos, primos);
    }else{
        tiempoFin = clock();
        tiempoTotal = ((double)tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
        printf("El número de primos en el rango %ld-%ld es %ld\n", datos[1], datos[2], *numeroPrimos);
        printf("Tiempo de cálculo del número de primos: %lf seg\n", tiempoTotal);
        fflush(stdout);
    }   
    close(s);
}


