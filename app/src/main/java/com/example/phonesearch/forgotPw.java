package com.example.phonesearch;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.button.MaterialButton;

public class forgotPw extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_forgot_pw);

        TextView username = (TextView) findViewById(R.id.usernameRt);

        MaterialButton retrieve = (MaterialButton) findViewById(R.id.retrievebtn);
        MaterialButton back = (MaterialButton) findViewById(R.id.backbtnforgot);

        retrieve.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String ctnUser = username.getText().toString();

                //search in the DB of user
                //if yes return password else announce that there is no
            }
        });

        back.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(forgotPw.this, Login.class);
                startActivity(intent);
            }
        });
    }
}