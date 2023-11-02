/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package clientetcp;
import java.net.*;
import java.io.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author alvar
 */
public class ClienteTCP {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Socket s = null;
        
        try {
            int serverPort = 7896;
            s = new Socket(args[1], serverPort);
            DataInputStream in = new DataInputStream(s.getInputStream());
            DataOutputStream out = new DataOutputStream(s.getOutputStream());
            out.writeUTF(args[0]);
            String data = in.readUTF();
            System.out.println("Reibido: " + data);
        } catch (UnknownHostException e) {
            System.out.println("Sock: " + e.getMessage());
        }catch(EOFException e){
            System.out.println("EOF: " + e.getMessage());
        }catch(IOException e){
            System.out.println("IO: " + e.getMessage());
        }finally{
            if(s!=null)
            try{
                s.close();
            }catch(IOException e){
                System.out.println("close: " + e.getMessage());
            }
            
        }
    }
    
}
