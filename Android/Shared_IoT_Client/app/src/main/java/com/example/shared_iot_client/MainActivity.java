package com.example.shared_iot_client;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    private final String host = client_config.SERVER_HOST_IP; //Aliyun ESC IP
    private final int port = client_config.SERVER_HOST_PORT;
    public static Socket_Assist clientTread=null;  //使用自定义的Socket_Assist类(同一路径）
    private Handler recv_handler=null;
    Button button_flogin;
    Button button_fregister;
    Button button_fappinfo;
//    private String[] demo;
//    private String[][] devinfo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome);
        new Handler((new Handler.Callback() {
            @Override
            public boolean handleMessage(@NonNull Message msg) {
                setContentView(R.layout.activity_main);
                Client_Init();
                return false;
            }
        })).sendEmptyMessageDelayed(0,2500);

//        String test="^req_adddev%username%devname%username%devname%username%devname%username%devname%$";
        if (!client_config.is_connected){
            try {
                clientTread = new Socket_Assist(recv_handler,host,port);
                client_config.is_connected=true;
                Toast.makeText(this,"server connected",Toast.LENGTH_SHORT).show();
            } catch (IOException e) {
                e.printStackTrace();
            }
            new Thread(clientTread).start();
        }


//        demo=ps.Parser_Mather(test);
//        demo=ps.Str_Copy_With_Pos(demo,1);
//        devinfo=ps.Couple_Pair(demo);
//        for(int i=0;i<devinfo.length;i++){
//            StringBuffer sb=new StringBuffer();
//            for(int j=0;j<devinfo[i].length;j++){
//                sb.append(devinfo[i][j]);
//            }
//            Log.i("devinfo "+i,sb.toString());
//
//        }


    }
    public void Client_Init(){
        button_flogin=(Button)findViewById(R.id.btn_flogin);
        button_fregister=(Button)findViewById(R.id.btn_fregister);
        button_fappinfo=(Button)findViewById(R.id.btn_fappinfo);
        button_flogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,LoginActivity.class);
                startActivity(intent);
            }
        });

        button_fregister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this,Activity_Register.class);
                startActivity(intent);
            }
        });

        button_fappinfo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this,"Shared_IoT_Client",Toast.LENGTH_SHORT).show();
//                Intent intent = new Intent(MainActivity.this,Client_MenuActivity.class);
//                startActivity(intent);
            }
        });
    }
//    public String[] Parser_Assit(String data){
//        String[] pres;
//        String mres;
//        String pattern="^\\^(.*)\\$$";
//        Pattern re=Pattern.compile(pattern);
//        Matcher matcher=re.matcher(data);
//
//        if(matcher.find()){
//            mres=matcher.group(1);
//            Log.i("res:",mres);
//            pres=mres.split("%");
//            return pres;
//        }
//        else {
//            return new String[]{};
//        }
//    }
}