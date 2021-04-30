package com.example.shared_iot_client;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;

import java.util.Timer;
import java.util.TimerTask;

public class Activity_welcome extends AppCompatActivity {
    private TimerTask task;
    private Timer timer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //non runable
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome);
        timer=new Timer();
        task=new TimerTask() {
            @Override
            public void run() {
                Intent intent=new Intent(Activity_welcome.this,MainActivity.class);
                startActivity(intent);
            }
       };
        timer.schedule(task,100,100);
    }
    @Override
    public void onStop() {
        super.onStop();
        timer.cancel();
    }
}