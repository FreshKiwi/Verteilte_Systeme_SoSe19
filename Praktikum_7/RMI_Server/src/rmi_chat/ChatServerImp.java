/*
 * Implementation of TimeServer.
 */
package rmi_chat;

import java.rmi.RemoteException;
import java.rmi.Naming;
import java.rmi.RMISecurityManager;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.registry.*;
import java.util.ArrayList;

public class ChatServerImp extends UnicastRemoteObject implements ChatServerInterface {
    ArrayList<ChatProxyImp> chatproxies;
    
    ChatServerImp() throws RemoteException {
        // Default ctor overloaded to catch RemoteException
        chatproxies = new ArrayList();
        System.out.println ("ChatServerImpl created...");
    }

    @Override
    public long getTime() throws RemoteException {
        return System.currentTimeMillis();
    }
    @Override
    public ChatProxy subscribeUser (String username, ClientProxy handle) throws RemoteException {
        return new ChatProxyImp();
    }

    public static void main(String[] args) {
        try {
            
            
            LocateRegistry.createRegistry (Registry.REGISTRY_PORT);
            
            Naming.rebind ("ChatServer", new ChatServerImp());
            System.out.println ("Class ChatServerImplementation registered as 'ChatServer' ...");
            
        } catch (Exception ex) {
            System.err.println ("Exception during server registration (port = " + Registry.REGISTRY_PORT + ")");
            ex.printStackTrace();
        }
    }
}


//System.setProperty("java.security.policy","file:///home/janbergm/AufgabenPraktikum/Verteilte_Systeme/Praktikum_7/RMI_Server/chat.policy");
//System.setSecurityManager( new RMISecurityManager() ) ;
            