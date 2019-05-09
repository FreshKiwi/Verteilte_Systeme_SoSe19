/*
 * Java interface of Time server.
 */
package rmi_chat;

import rmi_chat.ChatProxy;
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ChatServerInterface extends Remote {
    public ChatProxy subscribeUser (String username,ClientProxy handle)
        throws RemoteException;
    /*
    public boolean unsubscribeUser (String username)
        throws RemoteException;*/
    public long getTime() 
        throws RemoteException;
}

