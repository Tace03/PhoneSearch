package com.example.phonesearch;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.RatingBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.button.MaterialButton;

public class Ratings extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ratings);

        MaterialButton backbtnrating = (MaterialButton) findViewById(R.id.backbtnrating);
        TextView chooseproduct = (TextView) findViewById(R.id.inputproduct);
        RatingBar rate = (RatingBar) findViewById(R.id.ratingbar);
        TextView desProduct = (TextView) findViewById(R.id.descriptionrate);
        MaterialButton confirm = (MaterialButton) findViewById(R.id.confirmrating);

        //section 1: back btn (terminate the activity)
        //
        backbtnrating.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

        //section 2: collect input of user to choose the product to rate
        //remember to add things like suggestion also

        //section 3: rating of things with stars
        //need to learn here

        //section 4: collect input user in text about the product

        //section 5: confirmation of rating

        confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                float rating = rate.getRating();
                if(rating == 0){
                    Toast.makeText(Ratings.this, "Rating must be higher than 0!", Toast.LENGTH_SHORT).show();
                }

                String description  = desProduct.getText().toString();
                if(description.length() <8){
                    Toast.makeText(Ratings.this, "The description must have at least 8 characters", Toast.LENGTH_SHORT).show();
                }

                String prodRate = chooseproduct.getText().toString();
                if(prodRate.length() ==0){
                    Toast.makeText(Ratings.this, "You must choose a product in order to rate", Toast.LENGTH_SHORT).show();
                }

                if(rating != 0 && description.length()>=8 && prodRate.length()!=0){
                    //update these 3 details into the rating database
                    finish();
                }
            }
        });

    }
}