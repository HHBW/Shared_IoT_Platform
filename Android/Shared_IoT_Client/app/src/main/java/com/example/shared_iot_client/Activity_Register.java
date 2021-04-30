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

public class Activity_Register extends AppCompatActivity {

    private String rusername;
    private  String rpassword;
    private EditText ruser_input;
    private EditText rpassword_input;
    public Handler rrecv_handler=null;
    public Socket_Assist rclientTread=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity__register);
        Button button_sregister=(Button)findViewById(R.id.btn_sregister);
        Button button_rback=(Button)findViewById(R.id.btn_rback);
        ruser_input=(EditText)findViewById(R.id.ruser_input);
        rpassword_input=(EditText)findViewById(R.id.rpassword_input);
        rclientTread=MainActivity.clientTread;
        rrecv_handler =new Handler(){
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    Toast.makeText(Activity_Register.this,msg.obj.toString(),Toast.LENGTH_SHORT).show();
                }
            }
        };
        rclientTread.recv_handler=rrecv_handler;
        button_rback.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Activity_Register.this,MainActivity.class);
                startActivity(intent);
            }
        });
        button_sregister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                rusername=ruser_input.getText().toString();
                rpassword=rpassword_input.getText().toString();
                //Toast.makeText(Activity_Register.this,rusername+":"+rpassword,Toast.LENGTH_SHORT).show();
                Log.i("register",rusername);
                Message msg = rclientTread.send_handler.obtainMessage();
                msg.what=1; //send = 1
                msg.obj="^user_register"+"%"+rusername+"%"+rpassword+"%$";
                rclientTread.send_handler.sendMessage(msg);
            }
        });
    }
}