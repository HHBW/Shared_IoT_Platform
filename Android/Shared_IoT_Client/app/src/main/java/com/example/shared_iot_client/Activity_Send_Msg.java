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
import android.widget.TextView;

public class Activity_Send_Msg extends AppCompatActivity {
    public Socket_Assist smclientTread=null;
    Handler smrecv_handler=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity__send__msg);
        Button button_send=findViewById(R.id.btn_send);
        Button button_back=findViewById(R.id.btn_smback);
        TextView text_msgto=findViewById(R.id.text_sendto);
        EditText edit_send=findViewById(R.id.edit_sendmsg);
        smclientTread=MainActivity.clientTread;
        Intent intent=getIntent();
        String recvername=intent.getStringExtra("sendto");
        String text="发送给"+recvername+"的消息";
        text_msgto.setText(text);

        button_send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String send=edit_send.getText().toString();
                Send_Msg_Req(send,recvername);
            }
        });
        button_back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(Activity_Send_Msg.this,Manage_DevActivity.class);
                startActivity(intent);
            }
        });
        smclientTread.recv_handler=smrecv_handler;
    }

    private void Send_Msg_Req(String data,String recverrname){

        Message msg=smclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^send_msg_req%"+recverrname+"%"+data+"%$";
        Log.i("dev_send",msg.obj.toString());
        smclientTread.send_handler.sendMessage(msg);

    }

}