#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include <time.h>

#define MAXIMO 100000000

void *funcionPrimos(void *arg);

uint32_t primos[MAXIMO];
int opcion, nip;
uint32_t res;

uint32_t resultado;

struct parametros{
	int sd;
	int min;
	int max;
	int posicion;
	int *primos;
};

int main(int argc, char *argv[])
{
  const int SIZE_1S = 30000000;
  const int SIZE_2S = 30000000;
  const int SIZE_4S = 30000000;
  struct sockaddr_in server_addr1;
  struct hostent *hp;
  struct parametros p1;

  char *namehost;

  int puerto1, sd, mensaje, indice;
  int conectados = 0;
  int rango = 0;
  int num[4];

  /* Tratamiento de parámetros */
  namehost = argv[1];
  puerto1 = atoi(argv[2]);
  num[0] = htonl(atoi(argv[3])); //NIP
  num[1] = htonl(atoi(argv[4])); // DECISION 1 = CONTAR, 2 = ENUMERAR
  num[2] = htonl(atoi(argv[5])); //PRIMER NUMERO
  num[3] = htonl(atoi(argv[6])); //SEGUNDO NUMERO

  sd = socket(AF_INET, SOCK_STREAM, 0);

  // Se obtine y se rellena la dirección del servidor 
  bzero((char *)&server_addr1, sizeof(server_addr1));

  /* hp = gethostbyname("esquinazo.unizar.es"); */
  hp = gethostbyname(namehost);
  if (hp == NULL) {
    perror("Error gethostbyname");
  }

  bcopy (hp->h_addr, (char *)&(server_addr1.sin_addr), hp->h_length);
  server_addr1.sin_family = AF_INET;

  /*server_addr.sin_port = htons(4200); */
  server_addr1.sin_port = htons(puerto1);

  /* Se establece la conexión */
  if (connect(sd, (struct sockaddr *) &server_addr1, sizeof(server_addr1)) < 0) {
    perror("Error llamada connect");
  } else {
  	printf("Se ha extablecido conexión con el servidor: %d\n", puerto1);  
  }

  p1.primos = malloc(SIZE_2S *sizeof(int));

  opcion = num[1];
  nip = num[0];
  rango = ntohl(num[3]);

  p1.sd = sd;
  p1.min = num[2];
  p1.max = htonl(rango - 1);
  p1.posicion = 0;

  alarm(18000);
  funcionPrimos((void *)&p1);
  mensaje = alarm(0);

  if(ntohl(opcion) == 2){
  	printf("Primos encontrados: %d", ntohl(p1.primos[0]));
  	for(indice = 1; indice < resultado; indice++){
    	  printf(", %d", ntohl(p1.primos[indice]));
  	}
  	printf("\n");
  }

  res  = resultado;

  printf("Primos contados entre el intervalo %d y %d = %d\n",  ntohl(num[2]), ntohl(num[3]), res);
  printf("Tiempo empleado: %d segundos \n", 18000 - mensaje);
  fflush(stdout);
  exit(0);
}

void *funcionPrimos(void *arg){
  int escritos, leidos, total_read, bytes_read, mensaje;
  clock_t t_ini, t_fin;
  double secs;

  uint32_t res;
  char *desplazaVector;
  uint32_t num[4];

  struct parametros *p;
  p = (struct parametros *)arg;

	int sd = p -> sd;
	num[0] = nip;
	num[1] = opcion;
	num[2] = p -> min;
	num[3] = p -> max;
	int posicion = p -> posicion;

  /* envía la petición = opción elegida, minimo, maximo */
  t_ini = clock();
  escritos = write(sd, &num, 4 *sizeof(uint32_t));
  if(escritos < 0){
    close(sd);
    perror("Error escritura \n");
  }

  /* recibe la respuesta */
  leidos = read(sd, &res, sizeof(uint32_t));
  if(leidos < 0){
    close(sd);
    perror("Error lectura \n");
  }

  res = ntohl(res);
  resultado = res;

  if(ntohl(num[1]) == 2){
      desplazaVector = (char *)p -> primos;
      total_read = 0;
      bytes_read = 0;

      while (total_read < res*sizeof(uint32_t)) {
        bytes_read = read(sd, desplazaVector + total_read, res* sizeof(uint32_t) - total_read);
        total_read = total_read + bytes_read;

        if((bytes_read == 0) || (bytes_read == -1)) {
          close(sd);
	      perror("Error lectura \n");
      	}
      }
      t_fin = clock();
      secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  		printf("%.16g milisegundos ha tardado %d\n", secs * 1000.0, sd);
  }
  close(sd);
}
