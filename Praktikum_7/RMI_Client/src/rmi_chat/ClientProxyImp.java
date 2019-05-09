/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi_chat;

import java.rmi.RemoteException;
import java.rmi.RemoteException;
import java.rmi.Naming;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.registry.*;
/**
 *
 * @author janbergm
 */
public class ClientProxyImp extends UnicastRemoteObject implements ClientProxy {
    
    ClientProxyImp() throws RemoteException {
        
    }
    
    @Override
    public void receiveMessage (String username,String message) throws RemoteException{
            System.out.println(username+": "+message);
    }
        
}