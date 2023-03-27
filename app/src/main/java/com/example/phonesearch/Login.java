package com.example.phonesearch;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.button.MaterialButton;

public class Login extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_login);

        TextView username = (TextView) findViewById(R.id.usernamelg);
        TextView password = (TextView) findViewById(R.id.passwordlg);

        MaterialButton loginbtn = (MaterialButton) findViewById(R.id.loginbtn);
        MaterialButton createAcc = (MaterialButton) findViewById(R.id.createAcc);
        MaterialButton forgotpw = (MaterialButton) findViewById(R.id.forgotpw);

        //to check whether there is a connection to the DB
        boolean ifDB = false;

        loginbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String cntUser = username.getText().toString();
                String cntPass = password.getText().toString();

                if(ifDB){

                } else{
                    if(cntUser.equals("") && cntPass.equals("")){
                        Toast.makeText(Login.this, "All fields are mandatory", Toast.LENGTH_SHORT).show();
                    }
                    if(cntUser.equals("admin") && cntPass.equals("admin")) {
                        Toast.makeText(Login.this, "Login Successfully!", Toast.LENGTH_SHORT).show();
                        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                        startActivity(intent);
                    } else{
                        Toast.makeText(Login.this, "Invalid Credentials", Toast.LENGTH_SHORT).show();
                    }
                }

            }
        });

        createAcc.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Login.this, createAcc.class);
                startActivity(intent);
            }
        });


        forgotpw.setOnClickListener((new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(Login.this, forgotPw.class);
            }
        }));
    }


}