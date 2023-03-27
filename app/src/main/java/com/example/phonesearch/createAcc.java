package com.example.phonesearch;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.button.MaterialButton;

public class createAcc extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_acc);

        TextView username = (TextView) findViewById(R.id.usernamesg);
        TextView password = (TextView) findViewById(R.id.passwordsg);
        TextView cfPassword = (TextView) findViewById(R.id.cfpasswordsg);

        MaterialButton signupbtn = (MaterialButton) findViewById(R.id.signupbtn);
        MaterialButton backbtn = (MaterialButton) findViewById(R.id.backbtn);

        //upon creating a new
        signupbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //two conditions needed to be satisfied in this onClick
                // 1. the credentials have not appeared in the DB yet
                // 2. the password and confirm password needs to be identical
                String ctnUsername = username.getText().toString();
                String ctnPassword = password.getText().toString();
                String ctnCf = cfPassword.getText().toString();

                boolean ifInDB = false;
                boolean ifIden = false;

                //condition 1


                //condition 2
                if(ctnPassword.equals(ctnCf)) ifIden = true;

                //if both condition is satisfied, add to DB, return to the Login page
                // with successful notice
                //if either condition is false, return to the Login page
                //with unsuccessful notice
                if(ctnUsername.equals("") || ctnPassword.equals("") || ctnCf.equals("")){
                    Toast.makeText(createAcc.this, "All fields are mandatory", Toast.LENGTH_SHORT).show();
                }
                if(!ifInDB && ifIden){
                    Toast.makeText(createAcc.this, "New Account created!", Toast.LENGTH_SHORT).show();
                } else{
                    if(ifInDB){
                        Toast.makeText(createAcc.this, "The Credential is in DB already", Toast.LENGTH_SHORT).show();
                    } else{
                        Toast.makeText(createAcc.this, "Confirmation Password not Correct", Toast.LENGTH_SHORT).show();
                    }
                }

                Intent intent = new Intent(createAcc.this, Login.class);
                startActivity(intent);
            }
        });

        backbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(createAcc.this, Login.class);
                startActivity(intent);
            }
        });



    }
}