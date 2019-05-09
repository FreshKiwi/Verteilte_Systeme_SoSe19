/*
 * Client for TimeServer.
 */
package rmi_chat;

import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.*;

public class ChatClient extends UnicastRemoteObject{

    public static void main(String[] args) {
        try {
            System.out.println("ChatClient gestartet!");
            //System.setProperty("java.security.policy","file:///home/janbergm/AufgabenPraktikum/Verteilte_Systeme/Praktikum_7/RMI_Server/chat.policy");
            //System.setSecurityManager( new RMISecurityManager() ) ;
            //Securitymanager secman = new Securitymanager();
            Registry registry = LocateRegistry.getRegistry();
            
            ChatServerInterface csi = (ChatServerInterface) registry.lookup("ChatServer");
            long tm = csi.getTime(); 
            
           
            
            
            final ClientProxy clientproxy = new ClientProxyImp();
            //UnicastRemoteObject.exportObject(clientproxy,);
            
            ChatProxy chatproxy = csi.subscribeUser("jannik", clientproxy);
            
            chatproxy.sendMessage("hallo (:");
            
            System.out.println("TimeServer: " + tm + " (" + new Date (tm) + ")");
        } catch (AccessException e) {
            e.printStackTrace();
        } catch (RemoteException e) {
            e.printStackTrace();
        } catch (NotBoundException e) {
            e.printStackTrace();
        }
    }
}
