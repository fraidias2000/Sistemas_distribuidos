
package cliente;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ThreadCliente extends Thread {
    //Constantes
    private final int CONTAR_NUMEROS = 1;
    private final int LISTAR_NUMEROS = 2;
    private final int MAXIMO = 50000000;
    
    //Variables para la creación del socket
    Socket socket;
    ObjectInputStream entrada;
    ObjectOutputStream salida;
    String nombreHost;
    int puertoHost;
    
    //Variables usadas para la comunicación
    int numeroPrimos = 0;
    int datos[] = new int[3]; 
    int primos[] = new int[MAXIMO];
    
    //Variables para el calculo del tiempo
    private double tiempoInicio;
    private double tiempoFin;
    private double tiempoTotal;
    
    //Variable para identificar a cada thread
    private static int  identificador = 0;

    public double getTiempoTotal() {
        return tiempoTotal;
    }

    public static int getIdentificador() {
        return identificador;
    }

    public int getNumeroPrimos() {
        return numeroPrimos;
    }
    
    
    
    

    public ThreadCliente(String nombreHost, int puertoHost, int primerNumero, 
                          int ultimoNumero,int decision ) {
        this.nombreHost = nombreHost;
        this.puertoHost = puertoHost;
        datos[0] = primerNumero;
        datos[1] = ultimoNumero;
        datos[2] = decision;
        identificador ++ ;
    }
    @Override
    public void run() {  
        try {
            socket = new Socket(nombreHost, puertoHost);
            salida = new ObjectOutputStream(socket.getOutputStream());
            entrada = new ObjectInputStream(socket.getInputStream());
            
            if (datos[2] == CONTAR_NUMEROS){
                tiempoInicio = System.currentTimeMillis();
            
                salida.writeObject(datos);

                numeroPrimos = entrada.readInt();
               // primos = (int[]) entrada.readObject();

                tiempoFin = System.currentTimeMillis();
                tiempoTotal = (tiempoFin - tiempoInicio) / 1000.0;
		//System.out.println("Tiempo de cálculo del número de " + "primos: " + String.format("%.3f", tiempoTotal) + " segundos");
			
                socket.close();
                entrada.close();
                salida.close();
            
            }else if (datos[2] == LISTAR_NUMEROS ){
                tiempoInicio = System.currentTimeMillis();
            
                salida.writeObject(datos);

                numeroPrimos = entrada.readInt();
                primos = (int[]) entrada.readObject();

                tiempoFin = System.currentTimeMillis();
                tiempoTotal = (tiempoFin - tiempoInicio) / 1000.0;
		
			
                mostrarPrimos(numeroPrimos, primos);
                
                socket.close();
                entrada.close();
                salida.close();       
            }
        } catch (IOException | ClassNotFoundException ex) {
            System.err.println(ex.getLocalizedMessage());
            System.exit(1);
        }
        //System.out.println("Thread " + identificador + " : El número de primos en el rango " + datos[0] + "-" + datos[1] + " es " + numeroPrimos);
        //System.out.println("Tiempo de cálculo del número de primos: " + String.format("%.3f", tiempoTotal) + " segundos");
        
        
    }
    private static void mostrarPrimos(long numeroPrimos, int primos[]) {
        System.out.println("Primos devueltos por el servidor y tratados por el thread " + identificador + " :");
        int lineaLarga = 0;
        for (int i = 0; i < numeroPrimos; i++) {
            System.out.print(primos[i] + " ");
            lineaLarga++;
            if (lineaLarga == 40){
                System.out.print("\n ");
                lineaLarga = 0;
            }
        }
        System.out.println("");
        
    }
}