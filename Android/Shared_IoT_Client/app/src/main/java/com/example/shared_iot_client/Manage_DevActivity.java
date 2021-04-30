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
import android.widget.Toast;

import java.util.List;
import java.util.Map;

public class Manage_DevActivity extends AppCompatActivity {

    private ListView list_mdaddeddev;
    private ListView list_mdshareddev;
    public Handler mdrecv_handler=null;
    public Socket_Assist mdclientTread=null;
    private String[] initdevinfo;
    List<Map<String,Object>> addeddev_list;
    List<Map<String,Object>> shareddev_list;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manage__dev);
        Parse_Assist ps=new Parse_Assist();
        initdevinfo= new String[]{"none", "none","none"};
        list_mdaddeddev=(ListView) findViewById(R.id.list_addeddev);
        list_mdshareddev=(ListView) findViewById(R.id.list_shareddev);
        mdclientTread=MainActivity.clientTread;
        addeddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdaddeddev,initdevinfo,addeddev_list,1);
        shareddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdshareddev,initdevinfo,shareddev_list,1);
        mdrecv_handler=new Handler(){
            @SuppressLint("HandlerLeak")
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    String[] tmp;
                    String ahead;
                    tmp=ps.Parser_Mather(msg.obj.toString());
                    ahead=tmp[0];
                    Log.i("ahead",ahead);
//                    if(ahead=="ask_shared_dev"){  //not be equel
                    if(ahead.equals("ask_shared_dev")){
                        shareddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdshareddev,tmp,shareddev_list,1);
                    }
                    else if(ahead.equals("ask_added_dev")){

                        addeddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdaddeddev,tmp,addeddev_list,1);
                    }
                    else if(ahead.equals("ask_delete_dev")){
                        String ask;
                        ask=tmp[1];
                        if(ask.equals("successful")){
                            Toast.makeText(getApplicationContext(), "Delete Successfully",Toast.LENGTH_SHORT).show();
                            addeddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdaddeddev,tmp,addeddev_list,2);

                        }
                        else if(ask.equals("not_found")){
                            Toast.makeText(getApplicationContext(), "Dev Is Not Found",Toast.LENGTH_SHORT).show();
                        }
                        else{
                            Toast.makeText(getApplicationContext(), "Delete Failed",Toast.LENGTH_SHORT).show();
                        }
                    }
                    else if(ahead.equals("ask_cancel_dev")){
                        String ask;
                        ask=tmp[1];
                        if(ask.equals("successful")){
                            Toast.makeText(getApplicationContext(), "Cancel Successfully",Toast.LENGTH_SHORT).show();
                            shareddev_list=ps.Refresh_ListView(Manage_DevActivity.this,list_mdshareddev,tmp,shareddev_list,2);
                        }
                        else if(ask.equals("not_found")){
                            Toast.makeText(getApplicationContext(), "Dev Is Not Found",Toast.LENGTH_SHORT).show();
                        }
                        else{
                            Toast.makeText(getApplicationContext(), "Cancel Failed",Toast.LENGTH_SHORT).show();
                        }
                    }

                    //adddev_adapter.notifyDataSetChanged();
                }
            }
        };
        mdclientTread.recv_handler=mdrecv_handler;
        list_mdaddeddev.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                AlertDialog.Builder dialog=new AlertDialog.Builder(Manage_DevActivity.this,R.style.Theme_AppCompat_Dialog_Alert);
                dialog.setIcon(R.drawable.icon);
                dialog.setTitle("删除物联网设备");
                dialog.setMessage("是否删除该设备");
                String devname,ownername;
                Map<String, Object> map;
                map=addeddev_list.get(position);
                devname=map.get("devname").toString();
                ownername=map.get("ownername").toString();
                dialog.setPositiveButton("删除", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        Dev_Delete_Req(devname);

                    }
                });
                dialog.setNegativeButton("返回", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                dialog.setNeutralButton("发送消息", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        //Toast.makeText(Manage_DevActivity.this,"dev:"+devname+"owner:"+ownername,Toast.LENGTH_SHORT).show();
                        Intent intent=new Intent(Manage_DevActivity.this,Activity_Send_Msg.class);
                        intent.putExtra("sendto",ownername);
                        startActivity(intent);
                    }
                });
                dialog.create().show();
            }
        });
        list_mdshareddev.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                AlertDialog.Builder dialog=new AlertDialog.Builder(Manage_DevActivity.this,R.style.Theme_AppCompat_Dialog_Alert);
                dialog.setIcon(R.drawable.icon);
                dialog.setTitle("取消共享设备");
                dialog.setMessage("是否取消共享该设备");
                String devname,ownername;
                Map<String, Object> map;
                map=shareddev_list.get(position);
                devname=map.get("devname").toString();
                ownername=map.get("ownername").toString();
                dialog.setPositiveButton("是的", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        Dev_Cancle_Shared_Req(devname);
                    }
                });
                dialog.setNegativeButton("返回", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                dialog.setNeutralButton("设备信息", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Toast.makeText(Manage_DevActivity.this,"dev:"+devname+"owner:"+ownername,Toast.LENGTH_SHORT).show();
                    }
                });
                dialog.create().show();
            }
        });
        Dev_Manage_Page_Req();

    }
    private void Dev_Manage_Page_Req(){
        Message msg=mdclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_managedev_page"+"%$";
        Log.i("dev_send",msg.obj.toString());
        mdclientTread.send_handler.sendMessage(msg);
    }

    private void Dev_Delete_Req(String devname){
        Message msg=mdclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_delete_dev%"+devname+"%$";
        Log.i("dev_send",msg.obj.toString());
        mdclientTread.send_handler.sendMessage(msg);
    }
    private void Dev_Cancle_Shared_Req(String devname){
        Message msg=mdclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_cancel_dev%"+devname+"%$";
        Log.i("dev_send",msg.obj.toString());
        mdclientTread.send_handler.sendMessage(msg);
    }
}