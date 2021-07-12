package com.gmail.h34064090_hw3.newsreader

import android.app.ListActivity
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.ListView
import androidx.databinding.DataBindingUtil
import com.gmail.h34064090_hw3.newsreader.databinding.ActvitySwipeRefreshBinding


class MainActivity : ListActivity() {

    var news = mutableListOf<NewsData>()
    private lateinit var binding: ActvitySwipeRefreshBinding

    val swipeRefreshLayout by lazy {
//            findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        binding.swipeRefreshLayout
    }

    val adapter by lazy{
        MyAdapter(this, news)
    }//build adapter, this is activity, simple_list_..is textView

    override fun onListItemClick(l: ListView?, v: View?, position: Int, id: Long) {
        super.onListItemClick(l, v, position, id)

        val intent = Intent(this, PreviewActivity::class.java)
        intent.putExtra("title",news[position].title)
        intent.putExtra("description",news[position].description)

        startActivity(intent)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = DataBindingUtil.setContentView(this, R.layout.actvity_swipe_refresh)

        listAdapter = adapter

        swipeRefreshLayout.setOnRefreshListener {
            news.clear()
            downloadList() //refresh
        }

        downloadList()
    }

    private fun downloadList(){
        NewsSAX(object : ParserListener {

            override fun start() {
                runOnUiThread{
                    swipeRefreshLayout.isRefreshing = true
                }
            }

            override fun finish() {
                swipeRefreshLayout.isRefreshing = false
            }

            override fun setNewData(new: NewsData) {
                runOnUiThread{
                    news.add(new)
                    adapter.notifyDataSetChanged()
                }
            }
        }).parseURL("https://www.times-series.co.uk/news/national/rss/xml")
    }
}

