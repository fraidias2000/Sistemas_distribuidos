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

int opcion, nip;
uint32_t res;

uint32_t resultado[4];

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
  struct sockaddr_in server_addr1, server_addr2, server_addr3, server_addr4;
  struct hostent *hp;
  struct parametros p1, p2, p3, p4;

  char *namehost; 

  int puerto1, puerto2, puerto3, puerto4, indice, sd1, sd2, sd3, sd4, mensaje;
  int conectados = 0;
  int rango = 0;
  int num[4]; 
  
  pthread_t hilo, hilo2, hilo3, hilo1;

  namehost = argv[1];
  puerto1 = atoi(argv[2]);
  puerto2 = atoi(argv[3]);
  puerto3 = atoi(argv[4]);
  puerto4 = atoi(argv[5]);
  num[0] = htonl(atoi(argv[6]));
  num[1] = htonl(atoi(argv[7]));
  num[2] = htonl(atoi(argv[8]));
  num[3] = htonl(atoi(argv[9]));
  
  sd1 = socket(AF_INET, SOCK_STREAM, 0);
  sd2 = socket(AF_INET, SOCK_STREAM, 0);
  sd3 = socket(AF_INET, SOCK_STREAM, 0);
  sd4 = socket(AF_INET, SOCK_STREAM, 0);

  //PRIMER SERVIDOR

  /* Se obtine y se rellena la dirección del servidor */
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
  if (connect(sd1, (struct sockaddr *) &server_addr1, sizeof(server_addr1)) < 0) {
    perror("Error connect"); 
  } else {
  	printf("Se ha extablecido conexión con el servidor en el puerto: %d\n", puerto1);  
  	conectados = conectados + 1;
  }
  
  // SEGUNDO SERVIDOR
  
  bzero((char *)&server_addr2, sizeof(server_addr2));

  /* hp = gethostbyname("esquinazo.unizar.es"); */
  hp = gethostbyname(namehost);
  if (hp == NULL) {
    perror("Error gethostbyname");
  }
  
  bcopy (hp->h_addr, (char *)&(server_addr2.sin_addr), hp->h_length);
  server_addr2.sin_family = AF_INET;
  
  server_addr2.sin_port = htons(puerto2); 

  /* Se establece la conexión */  
  if (connect(sd2, (struct sockaddr *) &server_addr2, sizeof(server_addr2)) < 0) {
    perror("Error connect"); 
  } else {
  	printf("Se ha extablecido conexión con el servidor en el puerto: %d\n", puerto2);  
  	conectados = conectados + 1;
  }
  
  // TERCER SERVIDOR
  
  bzero((char *)&server_addr3, sizeof(server_addr3));

  /* hp = gethostbyname("esquinazo.unizar.es"); */
  hp = gethostbyname(namehost);
  if (hp == NULL) {
    perror("Error gethostbyname");
  }
  
  bcopy (hp->h_addr, (char *)&(server_addr3.sin_addr), hp->h_length);
  server_addr3.sin_family = AF_INET;
  
  server_addr3.sin_port = htons(puerto3); 

  /* Se establece la conexión */  
  if (connect(sd3, (struct sockaddr *) &server_addr3, sizeof(server_addr3)) < 0) {
    perror("Error connect"); 
  } else {
  	printf("Se ha extablecido conexión con el servidor en el puerto: %d\n", puerto3);  
  	conectados = conectados + 1;
  }
  
  // CUARTO SERVIDOR
  
  bzero((char *)&server_addr4, sizeof(server_addr4));

  /* hp = gethostbyname("esquinazo.unizar.es"); */
  hp = gethostbyname(namehost);
  if (hp == NULL) {
    perror("Error gethostbyname");
  }
  
  bcopy (hp->h_addr, (char *)&(server_addr4.sin_addr), hp->h_length);
  server_addr4.sin_family = AF_INET;
  
  server_addr4.sin_port = htons(puerto4); 

  /* Se establece la conexión */  
  if (connect(sd4, (struct sockaddr *) &server_addr4, sizeof(server_addr4)) < 0) {
    perror("Error connect"); 
  } else {
  	printf("Se ha extablecido conexión con el servidor en el puerto: %d\n", puerto4);  
  	conectados = conectados + 1;
  }
  
  p1.primos = malloc(SIZE_2S *sizeof(int));
  p2.primos = malloc(SIZE_2S *sizeof(int));
  p3.primos = malloc(SIZE_2S *sizeof(int));
  p4.primos = malloc(SIZE_2S *sizeof(int));
  
  if (conectados == 0){
  	printf("No se ha conectado a ningún puerto");
  	exit(0);
  }
  opcion = num[1];
  nip = num[0];
  rango = ntohl(num[3]) / conectados;
   	
  p1.sd = sd1;
  p1.min = num[2];
  p1.max = htonl(rango - 1);
  p1.posicion = 0;   
  
  p2.sd = sd2;
  p2.min = htonl(rango);
  p2.max = htonl(rango + rango - 1);
  p2.posicion = 1; 
  
  p3.sd = sd3;
  p3.min = htonl(rango + rango);
  p3.max = htonl(rango + rango + rango - 1);
  p3.posicion = 2; 
   
  p4.sd = sd4;
  p4.min = htonl(rango + rango + rango);
  p4.max = num[3];
  p4.posicion = 3; 
  
  alarm(18000);
  pthread_create(&hilo, NULL, funcionPrimos, (void *)&p1); 
  pthread_create(&hilo2, NULL, funcionPrimos, (void *)&p2);
  pthread_create(&hilo3, NULL, funcionPrimos, (void *)&p3); 
  pthread_create(&hilo1, NULL, funcionPrimos, (void *)&p4);
  	
  pthread_join(hilo, NULL);
  pthread_join(hilo2, NULL);
  pthread_join(hilo3, NULL);
  pthread_join(hilo1, NULL);
  mensaje = alarm(0);
       
  if(ntohl(opcion) == 2){	  
  	printf("Primos encontrados: %d", ntohl(p1.primos[0]));
  	for(indice = 1; indice < resultado[0]; indice++){
    	  printf(", %d", ntohl(p1.primos[indice]));
  	}
  	for(indice = 0; indice < resultado[1]; indice++){
    	  printf(", %d", ntohl(p2.primos[indice]));
  	}
  	for(indice = 0; indice < resultado[2]; indice++){
    	  printf(", %d", ntohl(p3.primos[indice]));
  	}
  	for(indice = 0; indice < resultado[3]; indice++){
    	  printf(", %d", ntohl(p4.primos[indice]));
  	}
  	printf("\n");
  }
  
  res  = resultado[0] + resultado[1] + resultado[2] + resultado[3];
  
  printf("Primos contados entre el intervalo %d y %d = %d\n",  ntohl(num[2]), ntohl(num[3]), res);
  printf("Tiempo empleado en la conexion: %d segundos \n", 18000 - mensaje);
  fflush(stdout);
  exit(0);
  
  
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
  resultado[posicion] = res;
  
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
