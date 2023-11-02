const MAXIMO_PRIMOS = 1000000;

struct primo {
   int numeroPrimos;
   int primos[MAXIMO_PRIMOS];
};

program PRIMOS {
   version UNO {
      int cuenta_primos(int min, int max) = 1;
      primo encuentra_primos(int min, int max) = 2;
   } = 1;
   version DOS {
      int cuenta_primos(int min, int max) = 1;
      primo encuentra_primos(int min, int max) = 2;
   } = 2;
   version TRES {
      int cuenta_primos(int min, int max) = 1;
      primo encuentra_primos(int min, int max) = 2;
   } = 3;
   version CUATRO {
      int cuenta_primos(int min, int max) = 1;
      primo encuentra_primos(int min, int max) = 2;
   } = 4;
} = 11;
