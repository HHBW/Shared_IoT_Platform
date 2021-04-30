package com.example.shared_iot_client;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.TextView;

import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

public class Activity_devdata_view extends AppCompatActivity {

    public Socket_Assist dvclientTread=null;
    Handler vdrecv_handler=null;
    String[][] devdata;
//    private TimerTask task;
//    private Timer timer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_devdata_view);
        TextView text_devtitle=findViewById(R.id.text_devtitle);
        TextView text_devdata=findViewById(R.id.text_devdata);
//        timer=new Timer();
//        Thread reqthread=new Thread(new Dev_Data_Req());
        Parse_Assist ps=new Parse_Assist();
        dvclientTread=MainActivity.clientTread;
        Intent intent=getIntent();
        String devname=intent.getStringExtra("devname");
        text_devtitle.setText(devname);
        vdrecv_handler=new Handler(){
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 2) {
                    String[] tmp;
                    String ahead;
                    tmp = ps.Parser_Mather(msg.obj.toString());
                    ahead = tmp[0];
                    if(ahead.equals("ask_dev_data")){
                        String ask_devname=tmp[1];
                        if(ask_devname.equals(devname)){
                            String tdata;
                            text_devdata.setText("");
                            devdata=ps.Couple_Pair_Data(tmp,2);
                            for(int i=0;i<devdata.length;i++){
                                tdata=devdata[i][0]+" "+devdata[i][1]+"\n";

                                text_devdata.append(tdata);
                                Log.i("tdata",tdata);
                            }

                        }
                    }
                }
            }
        };
        dvclientTread.recv_handler=vdrecv_handler;
//        task=new TimerTask() { 客户端不能决定什么时候接收设备数据,设备数据属于分享者定制
//            @Override
//            public void run() {
//                Dev_Data_Req();
//            }
//        };
//        timer.schedule(task,0,3000);
    }
    private synchronized void Dev_Data_Req(){

        Message msg=dvclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^dev_data_req%"+client_config.Client_Name+"%$";
        Log.i("dev_send",msg.obj.toString());
        dvclientTread.send_handler.sendMessage(msg);

    }
//    private class Dev_Data_Req implements Runnable{
//
//        @Override
//        public void run() {
//
//            Message msg=dvclientTread.send_handler.obtainMessage();
//            msg.what=1; //send = 1
//            msg.obj="^dev_data_req%"+client_config.Client_Name+"%$";
//            Log.i("dev_send",msg.obj.toString());
//            dvclientTread.send_handler.sendMessage(msg);
//        }
//    }
}