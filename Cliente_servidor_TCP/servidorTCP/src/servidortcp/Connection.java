/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package servidortcp;

import java.io.*;
import java.net.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alvar
 */
public class Connection extends Thread{
    DataInputStream in;
    DataOutputStream out;
    Socket clientSocket;
    public Connection(Socket aClienteSocket){
        try{
            clientSocket = aClienteSocket;
            in = new DataInputStream(clientSocket.getInputStream());
            out = new DataOutputStream(clientSocket.getOutputStream());
            this.start();
        }catch(IOException e){
            System.out.println("Connection: " + e.getMessage());
        
        }
    
    }
    public void run (){
        
        try {
            String data = in.readUTF();
            out.writeUTF(data);
        } catch (EOFException e) {
            System.out.println("EOF: " + e.getMessage());
        } catch(IOException e){
            System.out.println("IO: " + e.getMessage());
        } finally{
            try{
                clientSocket.close();
            }catch(IOException e){
                System.out.println("close failed");
            }
        
        
        
        }
        
    
    }
    
}
