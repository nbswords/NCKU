package com.gmail.h34064090_hw3.newsreader

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import com.gmail.h34064090_hw3.newsreader.databinding.NewreaderLayoutBinding


class MyAdapter(val context: Context, val news: List<NewsData>) :  BaseAdapter(){

    override fun getItem(position: Int): Any {
        return 0
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun getCount(): Int {
        return news.size
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val inflater = context.getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
        var binding: NewreaderLayoutBinding? = null
        var itemView = convertView

        if (convertView == null){
            // first run
            binding = NewreaderLayoutBinding.inflate(inflater, parent, false)
            itemView = binding.root
            itemView.tag = binding
        }else{
            binding = itemView?.tag as NewreaderLayoutBinding
        }
        binding.newsData = news[position]

        return itemView
    }

}

