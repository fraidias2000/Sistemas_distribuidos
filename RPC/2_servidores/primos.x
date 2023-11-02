
struct primo {
   int numeroPrimos;
   int listaPrimos[10000000];
};

program PRIMOS {
   version UNO {
      int cuentaPrimos(int min, int max) = 1;
      primo encuentraPrimos(int min, int max) = 2;
   } = 1;
   version DOS {
      int cuentaPrimos(int min, int max) = 1;
      primo encuentraPrimos(int min, int max) = 2;
   } = 2;
} = 11;
