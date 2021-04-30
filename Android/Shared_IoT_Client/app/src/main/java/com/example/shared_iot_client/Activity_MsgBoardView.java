package com.example.shared_iot_client;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.List;
import java.util.Map;
import java.util.Objects;

public class Activity_MsgBoardView extends AppCompatActivity {
    private ListView list_msgboard;
    Button button_fresh;
    private List<Map<String,Object>> msgboard_list;
    public Handler mbrecv_handler=null;
    public Socket_Assist mbclientTread=null;
    String[] sdata;
    String[] initdevinfo;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity__msg_board_view);
        Parse_Assist ps=new Parse_Assist();
        list_msgboard=findViewById(R.id.list_msgborad);
        button_fresh=findViewById(R.id.btn_mbfresh);
        initdevinfo= new String[]{"none", "none","none"};
        msgboard_list=ps.Refresh_MsgBoardList(Activity_MsgBoardView.this,list_msgboard,initdevinfo,msgboard_list,1);
        mbclientTread=MainActivity.clientTread;

        mbrecv_handler =new Handler(){
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    String ahead;
                    sdata=ps.Parser_Mather(msg.obj.toString());
                    ahead=sdata[0];
                    if(ahead.equals("ask_msgboard_page")) {
                        String tmp=sdata[1];
                        if(tmp.equals("none")){
                            Toast.makeText(Activity_MsgBoardView.this,"none msg",Toast.LENGTH_SHORT).show();
                            msgboard_list = ps.Refresh_MsgBoardList(Activity_MsgBoardView.this, list_msgboard, sdata, msgboard_list, 1);
                        }
                        else {
                            msgboard_list = ps.Refresh_MsgBoardList(Activity_MsgBoardView.this, list_msgboard, sdata, msgboard_list, 1);
                        }
                    }
                }
            }
        };
        mbclientTread.recv_handler=mbrecv_handler;

        list_msgboard.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                AlertDialog.Builder dialog=new AlertDialog.Builder(Activity_MsgBoardView.this,R.style.Theme_AppCompat_Dialog_Alert);
                dialog.setIcon(R.drawable.icon);
                dialog.setTitle("留言板");

                String sendername,sendermsg;
                Map<String, Object> map;
                map=msgboard_list.get(position);
                sendername= Objects.requireNonNull(map.get("sendername")).toString();
                sendermsg=Objects.requireNonNull(map.get("sendermsg")).toString();
                dialog.setMessage("删除或者回复"+sendername+"的消息");
                dialog.setPositiveButton("回复", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Intent intent=new Intent(Activity_MsgBoardView.this,Activity_Reply_Msg.class);
                        intent.putExtra("sendername",sendername);
                        intent.putExtra("sendermsg",sendermsg);
                        startActivity(intent);
                    }
                });
                dialog.setNegativeButton("返回", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                dialog.setNeutralButton("删除", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        User_Msg_Delete_Req(sendername);//全删,因为一个人发送的多个消息,可以统一回复
                        User_Msgboard_Page_Req();
                    }
                });
                dialog.create().show();

            }
        });
        button_fresh.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                User_Msgboard_Page_Req();
            }
        });
        User_Msgboard_Page_Req();
    }
    private void User_Msgboard_Page_Req(){
        Message msg=mbclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_msgboard_page"+"%$";
        Log.i("user_send",msg.obj.toString());
        mbclientTread.send_handler.sendMessage(msg);
    }
    private void User_Msg_Delete_Req(String sendername){
        Message msg=mbclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^user_msg_delete%"+client_config.Client_Name+"%"+sendername+"%$";
        Log.i("user_send",msg.obj.toString());
        mbclientTread.send_handler.sendMessage(msg);
    }
}