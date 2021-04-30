package com.example.shared_iot_client;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import java.util.List;
import java.util.Map;
import java.util.Objects;

public class View_DevActivity extends AppCompatActivity {

    private ListView list_vdaddeddev;
    public Socket_Assist vdclientTread=null;
    private String[] initdevinfo;
    List<Map<String,Object>> addeddev_list;
    public Handler vdrecv_handler=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view__dev);
        Parse_Assist ps=new Parse_Assist();
        initdevinfo= new String[]{"none", "none","none"};
        list_vdaddeddev=(ListView) findViewById(R.id.list_vddata);
        vdclientTread=MainActivity.clientTread;
        addeddev_list=ps.Refresh_ListView(View_DevActivity.this,list_vdaddeddev,initdevinfo,addeddev_list,1);
        vdrecv_handler=new Handler() {
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == 2) {  //recv = 2
                    String[] tmp;
                    String ahead;
                    tmp = ps.Parser_Mather(msg.obj.toString());
                    ahead = tmp[0];
                    Log.i("ahead", ahead);
//                    if(ahead=="ask_shared_dev"){  //not be equel
                    if (ahead.equals("ask_added_dev")) {
                        addeddev_list = ps.Refresh_ListView(View_DevActivity.this, list_vdaddeddev, tmp, addeddev_list, 1);
                    }
                }
            }
        };
        list_vdaddeddev.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String devname,ownername;
                Map<String, Object> map;
                map=addeddev_list.get(position);
                devname= Objects.requireNonNull(map.get("devname")).toString();
                Intent intent=new Intent(View_DevActivity.this,Activity_devdata_view.class);
                intent.putExtra("devname",devname);
                startActivity(intent);
            }
        });
        vdclientTread.recv_handler=vdrecv_handler;
        Dev_View_Req();
    }
    private void Dev_View_Req(){
        Message msg=vdclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_viewdev_page"+"%$";
        Log.i("dev_send",msg.obj.toString());
        vdclientTread.send_handler.sendMessage(msg);
    }
}