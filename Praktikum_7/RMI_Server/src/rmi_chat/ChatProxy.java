/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi_chat;

import java.rmi.Remote;
import java.rmi.RemoteException;
/**
 *
 * @author janbergm
 */
public interface ChatProxy extends Remote {
    public void sendMessage (String message)
        throws RemoteException;
}
