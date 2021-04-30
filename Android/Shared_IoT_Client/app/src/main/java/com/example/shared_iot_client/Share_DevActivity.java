package com.example.shared_iot_client;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Share_DevActivity extends AppCompatActivity {
    private String sdusername;
    private  String sddevname;
    private EditText sdusername_input;
    private EditText sddevname_input;
    public Handler sdrecv_handler=null;
    public Socket_Assist sdclientTread=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_share__dev);
        Parse_Assist ps=new Parse_Assist();
        Button button_sdok=(Button)findViewById(R.id.btn_sdok);
        Button button_sdcancel=(Button)findViewById(R.id.btn_sdcancel);
        sdusername_input=(EditText)findViewById(R.id.sdusername_input);
        sddevname_input=(EditText)findViewById(R.id.sddevname_input);
        sdclientTread=MainActivity.clientTread;

        sdrecv_handler =new Handler(){
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    String[] sdata;
                    sdata=ps.Parser_Mather(msg.obj.toString());
                    if(sdata.length==0) {
                        Toast.makeText(Share_DevActivity.this, msg.obj.toString(), Toast.LENGTH_SHORT).show();
                    }
                }
            }
        };
        sdclientTread.recv_handler=sdrecv_handler;
        button_sdok.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sdusername=sdusername_input.getText().toString();
                sddevname=sddevname_input.getText().toString();
                Log.i("dev_register",sddevname);
                Message msg = sdclientTread.send_handler.obtainMessage();
                msg.what=1; //send = 1
                msg.obj="^dev_register"+"%"+sddevname+"%"+sdusername+"%$";
                Log.i("dev_send",msg.obj.toString());
                sdclientTread.send_handler.sendMessage(msg);
            }
        });
        button_sdcancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Share_DevActivity.this,Client_MenuActivity.class);
                startActivity(intent);
            }
        });
    }
}