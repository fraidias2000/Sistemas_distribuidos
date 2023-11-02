
#include "funcpri.h"


int cuenta_primos(int min, int max)
{
  int i, contador;
  contador = 0;
  for (i = min; i <= max; i++)
    if (esprimo(i)) contador = contador + 1;
  return (contador);
}


int encuentra_primos(int min, int max, int vector[])
{
  int i, contador;
  contador = 0;
  for (i = min; i <= max; i++)
    if (esprimo(i)) vector[contador++] = i;
  return (contador);
}


int net_encuentra_primos(int min, int max, uint32_t vector[])
{
  int i, contador;
  contador = 0;
  for (i = min; i <= max; i++)
    if (esprimo(i)) vector[contador++] = htonl(i);
  return (contador);
}

int esprimo(int n)
{
  int i;
  for (i = 2; i*i <= n; i++)
    if ((n % i) == 0) return (0);
  return (1);
}

