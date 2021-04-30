package com.example.shared_iot_client;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.net.UnknownHostException;


public class Socket_Assist implements Runnable{
    //    private String SERVER_HOST_IP = "192.168.4.1"; //ESP IP
//    private int SERVER_HOST_PORT = 333;
    private String SERVER_HOST_IP;
    private int SERVER_HOST_PORT;
    public Socket clientsk=null;
    public Handler recv_handler=null;
    public OutputStream skoutput=null;
    private BufferedReader skinput=null;
    public Handler send_handler=null;
    public boolean SK_Established_Flag=false;
    public boolean SK_Send_Flag=false;
    public Socket_Assist(Handler handler,String Host_IP,int Host_Port) throws IOException {
        this.recv_handler=handler;//get the main thread handler
        this.SERVER_HOST_IP=Host_IP;
        this.SERVER_HOST_PORT=Host_Port;
    }
    @Override
    public void run() {
        try {
            clientsk = new Socket(SERVER_HOST_IP,SERVER_HOST_PORT);
            SK_Established_Flag=true;
            skoutput = clientsk.getOutputStream();
            //input=socket.getInputStream();
            skinput = new BufferedReader(new InputStreamReader(clientsk.getInputStream(),"utf-8"));
            //final String[] serverdata = {null};

            new Thread() {
                @Override
                public void run() {
                    try {
                        //String server_data = null;
                        char[] server_data=new char[1024];
                        int data_len=0;
                        while ((data_len=skinput.read(server_data))!=-1){
                            Message msg=recv_handler.obtainMessage();
                            msg.what=2;
                            msg.obj=new String(server_data,0,data_len);
                            Log.i("server",msg.obj.toString());
                            recv_handler.sendMessage(msg);

                        }

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }.start();
            Looper.prepare();
            send_handler=new Handler(){
                @Override
                public void handleMessage(Message msg){
                    if(msg.what==1){
                        try{
                            //skoutput.write((msg.obj.toString()+"\r\n").getBytes("utf-8"));
                            skoutput.write((msg.obj.toString()).getBytes("utf-8"));
                        } catch (UnsupportedEncodingException e) {
                            e.printStackTrace();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                        SK_Send_Flag=true;
                    }
                }
            };
            Looper.loop();
            //socket.close();
            //btnConnect.setEnabled(false);
            //btnSend.setEnabled(true);
            //editSend.setEnabled(true);
        }
        catch (UnknownHostException e){
            e.printStackTrace();
        }
        catch (IOException e){
            e.printStackTrace();
        }
    }

}
