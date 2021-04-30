package com.example.shared_iot_client;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Client_MenuActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_client__menu);
        Button button_fshare=(Button)findViewById(R.id.btn_fshare);
        Button button_fadd=(Button)findViewById(R.id.btn_fadd);
        Button button_fmanage=(Button)findViewById(R.id.btn_fmanage);
        Button button_fview=(Button)findViewById(R.id.btn_fview);
        Button button_fmsgboard=(Button)findViewById(R.id.btn_msgboard);

        button_fshare.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Client_MenuActivity.this,Share_DevActivity.class);
                startActivity(intent);
            }
        });
        button_fadd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Client_MenuActivity.this,Add_DevActivity.class);
                startActivity(intent);
            }
        });
        button_fmanage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Client_MenuActivity.this,Manage_DevActivity.class);
                startActivity(intent);
            }
        });
        button_fview.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Client_MenuActivity.this,View_DevActivity.class);
                startActivity(intent);
            }
        });
        button_fmsgboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent(Client_MenuActivity.this,Activity_MsgBoardView.class);
                startActivity(intent);
            }
        });

    }
}