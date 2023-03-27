package com.example.phonesearch;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //idk what is happening here
        super.onCreate(savedInstanceState);
        //at least i understand this is to connect to the main xml
        setContentView(R.layout.activity_main);
        Intent intent = new Intent(MainActivity.this, Login.class);
        startActivity(intent);
    }
}