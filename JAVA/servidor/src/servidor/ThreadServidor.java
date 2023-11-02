
package servidor;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ThreadServidor extends Thread {
    
    //Constantes
    public static final int MAXIMO = 50000000;
    private final int CONTAR_NUMEROS = 1;
    private final int LISTAR_NUMEROS = 2;

    //Variables para la creación del socket
    private Socket socket;
    private ObjectInputStream entrada;
    private ObjectOutputStream salida;
    
    //Variables usadas para la comunicación
    private int numeroPrimos; 
    private int datos[] = new int[3]; 
    private int primos[] = new int[MAXIMO];
    
    //Variables para el calculo del tiempo
    private double tiempoInicio;
    private double tiempoFin;
    private double tiempoTotal;
    
    //Variable para identificar a cada thread
    private int idThread;

    
    public ThreadServidor(Socket socket, int idThread) {
        this.socket = socket;
        this.idThread = idThread;
        try {
            entrada = new ObjectInputStream(socket.getInputStream());
            salida = new ObjectOutputStream(socket.getOutputStream());
        } catch (IOException ex) {
            System.err.println(ex.getLocalizedMessage());
        }
    }

    @Override
    public void run() {        
        try {
            datos = (int[]) entrada.readObject();
        
            if(datos[2] == CONTAR_NUMEROS){            
            
                tiempoInicio = System.currentTimeMillis();
            
                numeroPrimos = cuenta_primos(datos[0], datos[1]);
            
                tiempoFin = System.currentTimeMillis();
                tiempoTotal = (tiempoFin - tiempoInicio) / 1000.0;

                System.out.println("Thread nº" + idThread + " -> El número de primos en el rango " + datos[0] + "-" + datos[1] + " es " + numeroPrimos);

                System.out.println("Thread nº" + idThread + " -> Tiempo de cálculo del número de primos: " + String.format("%.3f", tiempoTotal) + " seg");

                salida.writeInt(numeroPrimos);
                salida.flush();
                
                socket.close();
                entrada.close();
                salida.close();
                System.out.println("Thread nº" + idThread + " ha acabado");       
            
            }else if (datos[2] == LISTAR_NUMEROS){
                
                tiempoInicio = System.currentTimeMillis();
            
                numeroPrimos = encuentra_primos(datos[0], datos[1], primos);
            
                tiempoFin = System.currentTimeMillis();
                tiempoTotal = (tiempoFin - tiempoInicio) / 1000.0;

                System.out.println("Thread nº" + idThread + " -> El número de primos en el rango " + datos[0] + "-" + datos[1] + " es " + numeroPrimos);

                System.out.println("Thread nº" + idThread + " -> Tiempo de cálculo del número de primos: " + String.format("%.3f", tiempoTotal) + " seg");                

                mostrarPrimos();
                
                salida.writeInt(numeroPrimos);
                salida.writeObject(primos);
                salida.flush();

                socket.close();
                entrada.close();
                salida.close();
                System.out.println("Thread nº" + idThread + " ha acabado");  
            }
        } catch (IOException ex) {
            Logger.getLogger(ThreadServidor.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(ThreadServidor.class.getName()).log(Level.SEVERE, null, ex);
        }         
    }

    private void mostrarPrimos() {
            System.out.print("Thread nº" + idThread + " -> Primos encontrados: ");
            for (int i = 0; i < numeroPrimos; i++) {
                System.out.print(primos[i] + " ");
            }
            System.out.println("");
        
    }
    public int cuenta_primos(int min, int max) {
        int i, contador;
        contador = 0;

        for (i = min; i <= max; i++) {
            if (esprimo(i)) {
                contador = contador + 1;
            }
        }
        return (contador);
    }

    /**
     * Encuentra los primos mediante el rango y el vector pasado por parámetro
     * 
     * @param min número
     * @param max número
     * @param vector de primos
     * @return long primos encontrados
     */
    public int encuentra_primos(int min, int max, int vector[]) {
        int contador; //En la versión de C era long
        contador = 0;

        for (int i = min; i <= max; i++) {
            if (esprimo(i)) {
                vector[contador++] = i;
            }
        }
        return (contador);
    }

    /**
     * Función que indica si el número pasado por parámetro es primo o no
     * 
     * @param número
     * @return booleano verdadero si es primo, falso en caso contrario
     */
    public boolean esprimo(int n) {
        int i;

        for (i = 2; i * i <= n; i++) {
            if ((n % i) == 0) {
                return false;
            }
        }

        return true;
    }
    
}