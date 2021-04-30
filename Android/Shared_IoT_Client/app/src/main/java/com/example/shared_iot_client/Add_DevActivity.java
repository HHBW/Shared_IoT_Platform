package com.example.shared_iot_client;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Add_DevActivity extends AppCompatActivity {

    private ListView list_adddev;
    Button button_fresh;
    private List<Map<String,Object>> adddev_list;
    public Handler adrecv_handler=null;
    public Socket_Assist adclientTread=null;
    String[] sdata;
    String[] initdevinfo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add__dev);
        Parse_Assist ps=new Parse_Assist();
        list_adddev=findViewById(R.id.list_adddev);
        button_fresh=findViewById(R.id.btn_fresh);
        initdevinfo= new String[]{"none", "none","none"};
        adddev_list=ps.Refresh_ListView(Add_DevActivity.this,list_adddev,initdevinfo,adddev_list,1);
        adclientTread=MainActivity.clientTread;

        adrecv_handler =new Handler(){
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    String ahead;
                    sdata=ps.Parser_Mather(msg.obj.toString());
                    ahead=sdata[0];
                    if(ahead.equals("ask_adddev_page")) {

                        adddev_list=ps.Refresh_ListView(Add_DevActivity.this,list_adddev,sdata,adddev_list,1);
                    }
                    else if(ahead.equals("ask_add_dev")){
                        String ask;
                        ask=sdata[1];
                        if(ask.equals("successful")){
                            Toast.makeText(getApplicationContext(), "Add Successfully",Toast.LENGTH_SHORT).show();
                        }
                        else if(ask.equals("isexist")){
                            Toast.makeText(getApplicationContext(), "Dev Is Exist",Toast.LENGTH_SHORT).show();
                        }
                        else if(ask.equals("not_found")){
                            Toast.makeText(getApplicationContext(), "Dev Not Found",Toast.LENGTH_SHORT).show();
                        }
                        else{
                            Toast.makeText(getApplicationContext(), "Add Failed",Toast.LENGTH_SHORT).show();
                        }
                    }
                    //adddev_adapter.notifyDataSetChanged();
                }
            }
        };
        adclientTread.recv_handler=adrecv_handler;
        list_adddev.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                AlertDialog.Builder dialog=new AlertDialog.Builder(Add_DevActivity.this,R.style.Theme_AppCompat_Dialog_Alert);
                dialog.setIcon(R.drawable.icon);
                dialog.setTitle("添加物联网设备");
                dialog.setMessage("是否添加该设备");
                //dialog.setCancelable(false);
                String devname,ownername;
                Map<String, Object> map;
                map=adddev_list.get(position);
                devname=map.get("devname").toString();
                ownername=map.get("ownername").toString();
                dialog.setPositiveButton("是的", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        Dev_Add_Req(devname,ownername);
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
                        Toast.makeText(Add_DevActivity.this,"dev:"+devname+"owner:"+ownername,Toast.LENGTH_SHORT).show();
                    }
                });
                dialog.create().show();
            }
        });
        button_fresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Dev_Add_Page_Req();
            }
        });
        Dev_Add_Page_Req();
    }

    private void Dev_Add_Page_Req(){
        Message msg=adclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_adddev_page"+"%$";
        Log.i("dev_send",msg.obj.toString());
        adclientTread.send_handler.sendMessage(msg);
    }

    private void Dev_Add_Req(String devname,String ownername){
        Message msg=adclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_add_dev%"+devname+'%'+ownername+"%$";
        Log.i("dev_send",msg.obj.toString());
        adclientTread.send_handler.sendMessage(msg);
    }
}

//    private List<Map<String, Object>> getData(int resid,String[][] devinfo) {
//        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
//        Map<String, Object> map = new HashMap<String, Object>();
//        map.put("devname", "G1");
//        map.put("ownername", "google 1");
//        map.put("img", resid);
//        list.add(map);
//
//        map = new HashMap<String, Object>();
//        map.put("devname", "G1");
//        map.put("ownername", "google 1");
//        map.put("img", resid);
//        list.add(map);
//
//        map = new HashMap<String, Object>();
//        map.put("devname", "G1");
//        map.put("ownername", "google 1");
//        map.put("img", resid);
//        list.add(map);
//
//        return list;
//    }