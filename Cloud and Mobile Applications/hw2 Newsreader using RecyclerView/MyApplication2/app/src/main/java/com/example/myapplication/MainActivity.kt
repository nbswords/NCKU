package com.example.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView


class MainActivity : AppCompatActivity() {

    private lateinit var recyclerView: RecyclerView
    private lateinit var viewManager: RecyclerView.LayoutManager
    private lateinit var viewAdapter: MyAdapter

    var newsTitle = mutableListOf<String>()
    var newsDescription = mutableListOf<String>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        viewManager= LinearLayoutManager(this)
        viewAdapter= MyAdapter(newsTitle, newsDescription)
        recyclerView = findViewById<RecyclerView>(R.id.recyclerView).apply {
            layoutManager =viewManager
            adapter = viewAdapter
        }
        newsParser()
    }

    private fun newsParser() {
        NewsSAX(object : ParserListener {

            override fun setTitle(title: String) {
                runOnUiThread{
                    Log.i("Title:", title)
                    newsTitle.add(title)
                }

            }
            override fun setDescription(description: String) {
                runOnUiThread {
                    Log.i("Description:", description)
                    newsDescription.add(description)
                    viewAdapter.notifyDataSetChanged()
                    // Notify Adapter to update view
                }

            }

        }).parseURL("https://www.times-series.co.uk/news/national/rss/")

    }

}
