/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi_chat;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;

/**
 *
 * @author janbergm
 */
public class ChatProxyImp extends UnicastRemoteObject implements ChatProxy{
    
    
    
    public ChatProxyImp() throws RemoteException{
        
    }
    
    public void sendMessage (String message)throws RemoteException{
        
    }
}
