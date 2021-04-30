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

import java.io.IOException;

public class LoginActivity extends AppCompatActivity {

    private String lusername;
    private  String lpassword;
    private EditText luser_input;
    private EditText lpassword_input;
    public Handler lrecv_handler=null;
    public Socket_Assist lclientTread=null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        Button button_slogin=(Button)findViewById(R.id.btn_slogin);
        Button button_lback=(Button)findViewById(R.id.btn_lback);
        Parse_Assist ps=new Parse_Assist();
        luser_input=(EditText)findViewById(R.id.luser_input);
        lpassword_input=(EditText)findViewById(R.id.lpassword_input);
        lclientTread=MainActivity.clientTread;
        lrecv_handler =new Handler(){
            @Override
            public void handleMessage(Message msg){
                if(msg.what==2){  //recv = 2
                    String[] tmp;
                    String ahead,username;
                    username=luser_input.getText().toString();
                    tmp = ps.Parser_Mather(msg.obj.toString());
                    ahead = tmp[0];
                    if(ahead.equals("ask_user_login")){
                        if(tmp[1].equals("successful")){
                            client_config.Client_Name=username;
                            Toast.makeText(LoginActivity.this,username+" successfully",Toast.LENGTH_SHORT).show();
                            Intent intent=new Intent(LoginActivity.this,Client_MenuActivity.class);
                            startActivity(intent);
                        }
                        else if(tmp[1].equals("is exist")) {
                            Toast.makeText(LoginActivity.this,username+" is logged or pwd error",Toast.LENGTH_SHORT).show();
                        }
                        else{
                            Toast.makeText(LoginActivity.this,username+" is failed",Toast.LENGTH_SHORT).show();
                        }
                    }

                }
            }
        };
        lclientTread.recv_handler=lrecv_handler;
        button_lback.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this,MainActivity.class);
                startActivity(intent);
            }
        });
        button_slogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                lusername=luser_input.getText().toString();
                lpassword=lpassword_input.getText().toString();
                //                    clientTread = new Socket_Assist(recv_handler,SERVER_HOST_IP,SERVER_HOST_PORT);
//                    new Thread(clientTread).start();
                Log.i("login",lusername);
                Message msg = lclientTread.send_handler.obtainMessage();
                msg.what=1; //send = 1
                msg.obj="^user_login"+"%"+lusername+"%"+lpassword+"%$";
                lclientTread.send_handler.sendMessage(msg);
                //Toast.makeText(LoginActivity.this,lusername+":"+lpassword,Toast.LENGTH_SHORT).show();
            }
        });
    }
}