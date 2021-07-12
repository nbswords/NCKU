package com.example.qrcode

import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button

class MainActivity : AppCompatActivity() {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

//      Scan QR Code
        findViewById<Button>(R.id.scan).setOnClickListener {
            startActivity(Intent(this, ScanQR::class.java))
        }
//      Generate QR Code
        findViewById<Button>(R.id.generate).setOnClickListener {
            startActivity(Intent(this, GenerateQR::class.java))
        }

    }
}



