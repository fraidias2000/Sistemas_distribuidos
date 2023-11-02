#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "funcpri.h"
#include <time.h>

#define PUERTO_SERVIDOR 4400
#define MAXIMO_PRIMOS 1000000

int s;

void finalizarServidor();    /* Captura la señal de interrupción SIGINT */
void mostrarPrimos(int idHijo, int numeroPrimos, int primos[]);

int main() {
    struct sockaddr_in server_addr, direccionCliente;
    int sc, pid;
    unsigned int sizeDireccion;
    int numeroPrimos;   
    int datos[3]; 
    int primos[MAXIMO_PRIMOS];
    int numeroHijos = 0;
    int idHijo = 0;

    /* Evitamos que los hijos se queden zombis */
    signal(SIGCLD, SIG_IGN);
    signal(SIGINT, finalizarServidor);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("\nError creando socket");
        return 1;
    }

    /* Asigna la dirección al socket */
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PUERTO_SERVIDOR);

    /* Si el puerto ya está es uso finaliza el servidor */
    if (connect(s, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("El puerto %d está disponible", PUERTO_SERVIDOR);
    } else {
        printf("Error el puerto %d está actualmente en uso\n", PUERTO_SERVIDOR);
        exit(1);
    }

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("\nError en bind");
        close(s);
        exit(1);
    }

    if (listen(s, 5) < 0) {
        perror("\nError en listen");
        close(s);
        exit(1);
    }

    sizeDireccion = sizeof(direccionCliente);

    printf("Servidor ejecutándose en el puerto: %d\n", PUERTO_SERVIDOR);
    fflush(stdout);

    while (1) {

        if ((sc = accept(s, (struct sockaddr *)&direccionCliente, &sizeDireccion)) < 0) {
            perror("\nError accept");
            close(s);
            exit(1);
        }

        pid = fork();
        numeroHijos = numeroHijos + 1;

        switch (pid) {
            case -1:
                perror("\nError al crear un hijo");
                close(sc);
                exit(1);

            case 0:
                idHijo = numeroHijos;
                close(s);

                //Recibe la información
                if (read(sc, &datos, sizeof(datos)) < 0) {
                    perror("\nError read");
                    close(sc);
                    exit(1);
                }
                numeroPrimos = encuentra_primos(datos[1], datos[2], primos);
                printf("El numero de primos es %d", numeroPrimos);

                //Envia el número de primos
                if (write(sc, &numeroPrimos, sizeof(int)) < 0) {
                    perror("\nError write");
                    close(sc);
                    exit(1);
                }
                if(datos[0] == 2){
                    // Envía los primos
                    ssize_t total_bytes_mandados = 0;
                    size_t restante = sizeof(primos);

                    while (total_bytes_mandados !=  sizeof(primos)) {
                        ssize_t bytes_mandados = write(sc, &primos[total_bytes_mandados], restante);
                        if (bytes_mandados == -1) {
                            perror("\nError write");
                            close(sc);
                            exit(1);
                        }
                        restante -= total_bytes_mandados;
                        total_bytes_mandados += bytes_mandados;
                    }
                    mostrarPrimos(idHijo, numeroPrimos, primos);
                }
                close(sc);
                printf("Hijo nº%d ha finalizado\n", idHijo);
                fflush(stdout);
                exit(0);

            default:
                printf("Esperando un nuevo cliente\n");
        }

    }

    return 0;
}
void finalizarServidor() {
    signal(SIGINT, SIG_IGN);
    printf("\nEl servidor ha finalizado\n");
    close(s);
    exit(0);
   
}
void mostrarPrimos(int idHijo, int numeroPrimos, int primos[]) {
        printf("Hijo nº%d -> Primos encontrados: ", idHijo);
        int i = 0;
        for (i = 0; i < numeroPrimos; i++) {
            printf("%d ", primos[i]);
        }
        printf("\n");
        fflush(stdout);
}
