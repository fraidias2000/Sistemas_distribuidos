
#include <stdio.h>
#include <netinet/in.h>

#define MINIMO 1L
#define MAXIMO 51000000L

/* Cuenta los primos entre min y max */
int cuenta_primos(int min, int max);
// long cuenta_primos(long min, long max);		//

/* Cuenta y encuentra los primos entre min y max y los devuelve en un vector */
int encuentra_primos(int min, int max, int vector[]);
// long encuentra_primos(long min, long max, long vector[]);		//
int net_encuentra_primos(int min, int max, uint32_t vector[]);
// long net_encuentra_primos(long min, long max, uint32_t vector[]);		//

/* Devuelve TRUE si n es primo */
int esprimo(int n);
// int esprimo(long n);		//


