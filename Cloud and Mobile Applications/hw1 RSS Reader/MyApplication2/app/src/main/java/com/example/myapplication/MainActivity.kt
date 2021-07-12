package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.LinearLayout
import android.widget.TextView

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val linearLayout:LinearLayout = findViewById(R.id.linearLayout)

        iTuneSAX(object: ParserListener{
            override fun setTitle(title: String) {
                Log.i("Title", title)

                runOnUiThread{
                    val textView = TextView(this@MainActivity)
                    textView.text = title
                    linearLayout.addView(textView)
                }
            }

        }).parseURL("https://www.times-series.co.uk/news/national/rss/")
        // https://www.times-series.co.uk/news/national/rss/
        // http://ax.itunes.apple.com/WebObjects/MZStoreServices.woa/ws/RSS/topsongs/limit=25/xml
    }
}
