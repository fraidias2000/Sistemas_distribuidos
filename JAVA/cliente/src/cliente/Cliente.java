package cliente;

import java.util.Scanner;

public class Cliente {
    //Variables para la creación del socket
    private static final int PUERTO = 4400;
    private static final String URL = "localhost"; 
    
 
    public static void main(String[] args) throws InterruptedException {
        Scanner teclado = new Scanner(System.in);
        int datos[] = new int[4];
        
        System.out.println("Dime el primer numero");
        datos[0] = Integer.parseInt(teclado.nextLine());
        
        System.out.println("Dime el segundo numero");
        datos[1] = Integer.parseInt(teclado.nextLine());
        
        System.out.println("Dime que quieres hacer (1= contar , 2 = listar)");
        datos[2] = Integer.parseInt(teclado.nextLine());
        
        System.out.println("¿Con cuantos servidores quieres hacerlo? (1,2 o 4)");
        datos[3]= Integer.parseInt(teclado.nextLine());
           
        switch (datos[3]) {
                
                // 1 SERVIDOR
                case 1:
                    System.out.println("El cliente enlazará con 1 servidor");
                    ThreadCliente cliente1_1servidor = new ThreadCliente(URL, PUERTO, datos[0], datos[1], datos[2]);
                    
                    cliente1_1servidor.start();
                    
                    while (cliente1_1servidor.isAlive()) {
                        Thread.sleep(20);
                    }       
                    //Separacion
                    System.out.println("");
                    //Datos 1 servidor
                    System.out.println("EL tiempo tardado por el thread " + cliente1_1servidor.getId() + " es: " + cliente1_1servidor.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente1_1servidor.getId() + " es: " +cliente1_1servidor.getNumeroPrimos() );
                    break;
                    
                // 2 SERVIDORES
                case 2:
                    System.out.println("El cliente enlazará con 2 servidores");
                    ThreadCliente cliente1_2servidores = new ThreadCliente(URL,   PUERTO, datos[0], datos[1] / 2, datos[2]);
                    ThreadCliente cliente2_2servidores = new ThreadCliente(URL, PUERTO, (datos[0] / 2) + 1, datos[1], datos[2]);
                    
                    cliente1_2servidores.start();
                    cliente2_2servidores.start();
                    
                    while (cliente1_2servidores.isAlive() || cliente2_2servidores.isAlive()) {
                        Thread.sleep(20);
                    }
                     //Separacion
                    System.out.println("");
                    //Datos primer Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente1_2servidores.getId() + " es: " + cliente1_2servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente1_2servidores.getId() + " es: " +cliente1_2servidores.getNumeroPrimos() );
                    
                    //Separacion
                    System.out.println("");
                    
                    //Datos segundo Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente2_2servidores.getId() + " es: " + cliente2_2servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente2_2servidores.getId() + " es: " +cliente2_2servidores.getNumeroPrimos() );
                    break;
                
                // 4 SERVIDORES
                case 4:
                    System.out.println("El cliente enlazará con 4 servidores");
                    ThreadCliente cliente1_4servidores = new ThreadCliente(URL, PUERTO, datos[0],  datos[1]  / 4, datos[2]);
                    ThreadCliente cliente2_4servidores = new ThreadCliente(URL, PUERTO, (datos[1] / 4) + 1, (datos[1]  / 4) * 2, datos[2]);
                    ThreadCliente cliente3_4servidores = new ThreadCliente(URL, PUERTO, (datos[1]  / 4) * 2 + 1, (datos[1]  / 4) * 3, datos[2]);
                    ThreadCliente cliente4_4servidores = new ThreadCliente(URL, PUERTO, (datos[1]  / 4) * 3 + 1, datos[1] , datos[2]);
                    
                    cliente1_4servidores.start();
                    cliente2_4servidores.start();
                    cliente3_4servidores.start();
                    cliente4_4servidores.start();
                    
                    while (cliente1_4servidores.isAlive() || cliente2_4servidores.isAlive() || cliente3_4servidores.isAlive() || cliente4_4servidores.isAlive()) {
                        Thread.sleep(20);
                    }
                    //Separacion
                    System.out.println("");
                    //Datos primer Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente1_4servidores.getId() + " es: " + cliente1_4servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente1_4servidores.getId() + " es: " +cliente1_4servidores.getNumeroPrimos() );
                    
                    //Separacion
                    System.out.println("");
                    
                    //Datos segundo Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente2_4servidores.getId() + " es: " + cliente2_4servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente2_4servidores.getId() + " es: " +cliente2_4servidores.getNumeroPrimos() );
                    
                    //Separacion
                    System.out.println("");
                    
                    //Datos tercer Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente3_4servidores.getId() + " es: " + cliente3_4servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente3_4servidores.getId() + " es: " +cliente3_4servidores.getNumeroPrimos() );
                    
                    //Separacion
                    System.out.println("");
                    
                    //Datos cuarto Thread
                    System.out.println("EL tiempo tardado por el thread " + cliente4_4servidores.getId() + " es: " + cliente4_4servidores.getTiempoTotal() + " segundos");
                    System.out.println("Cantidad de primos encontrada por el thread " + cliente4_4servidores.getId() + " es: " +cliente4_4servidores.getNumeroPrimos() );
                    break;
            }
    }   
}