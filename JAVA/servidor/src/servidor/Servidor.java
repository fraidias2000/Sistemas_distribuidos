package servidor;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class Servidor {
    private static final int PUERTO = 4400;
    
    public static void main (String [ ] args) {
        try {
            ServerSocket servidor = new ServerSocket(PUERTO);
            Socket socket;
            ThreadServidor esclavo;
            int numeroThreads = 0;

            System.out.println("Servidor para el calculo de primos funcionando");
            System.out.println("Escuchando el puerto " + PUERTO);
            System.out.println("A la espera de nuevos clientes...");

            while (true) {
                socket = servidor.accept();
                System.out.println("Se ha creado un Thread para atender a un cliente");
                esclavo = new ThreadServidor(socket, numeroThreads++);
                esclavo.start();
            }
        } catch (IOException ex) {
            System.err.println("Puerto " + PUERTO + " en uso.");
        }
    }
}