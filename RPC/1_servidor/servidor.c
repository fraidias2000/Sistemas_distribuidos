/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "primos.h"

int *
cuenta_primos_1_svc(int min, int max,  struct svc_req *rqstp)
{
	static int  result;
    result = 0;

    for (int i = min; i <= max; i++)
        if (esprimo(i)) {
            result = result + 1;
        }
	return &result;
}

primo *
encuentra_primos_1_svc(int min, int max,  struct svc_req *rqstp)
{
	static primo  result;
    result.numeroPrimos = 0;

    for (int i = min; i <= max; i++)
        if (esprimo(i)) {
            result.primos[result.numeroPrimos++] = i;
        }
	return &result;
}
int esprimo(int n) {
    int i;

    for (i = 2; i * i <= n; i++)
        if ((n % i) == 0) {
            return (0);
        }

    return (1);
}