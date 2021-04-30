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

public class Activity_Reply_Msg extends AppCompatActivity {
    public Socket_Assist rmclientTread=null;
    Handler rmrecv_handler=null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity__reply__msg);
        Button button_reply=findViewById(R.id.btn_reply);
        Button button_back=findViewById(R.id.btn_rmback);
        TextView text_msgfrom=findViewById(R.id.text_msgfrom);
        TextView text_msgdata=findViewById(R.id.text_msgdata);
        EditText edit_reply=findViewById(R.id.edit_msgreply);
        Parse_Assist ps=new Parse_Assist();
        rmclientTread=MainActivity.clientTread;
        Intent intent=getIntent();
        String sendername=intent.getStringExtra("sendername");
        String sendermsg=intent.getStringExtra("sendermsg");
        String text="来自"+sendername+"的消息";
        text_msgfrom.setText(text);
        text_msgdata.setText(sendermsg);
        button_reply.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String reply=edit_reply.getText().toString();
                Reply_Msg_Req(reply,sendername);
            }
        });
        button_back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(Activity_Reply_Msg.this,Activity_MsgBoardView.class);
                startActivity(intent);
            }
        });
        rmclientTread.recv_handler=rmrecv_handler;
    }

    private void Reply_Msg_Req(String data,String sendername){

        Message msg=rmclientTread.send_handler.obtainMessage();
        msg.what=1; //send = 1
        msg.obj="^reply_msg_req%"+sendername+"%"+data+"%$";
        Log.i("dev_send",msg.obj.toString());
        rmclientTread.send_handler.sendMessage(msg);

    }
}