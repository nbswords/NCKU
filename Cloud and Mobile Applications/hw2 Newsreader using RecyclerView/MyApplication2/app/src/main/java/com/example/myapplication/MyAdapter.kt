package com.example.myapplication

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

// 傳入Array, 繼承RecyclerView, 告訴它ViewHolder名稱
class MyAdapter(private val newsTitle: List<String>,private  val newsDescription: List<String>): RecyclerView.Adapter<MyAdapter.MyViewHolder>() {

    class MyViewHolder(view: View):RecyclerView.ViewHolder(view){
        val newsTitle: TextView = view.findViewById(R.id.Title)
        val newsDescription: TextView = view.findViewById(R.id.Description)
    }

    // 當RecyclerView需要view時, 這個func幫它建立所需要的view
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): MyViewHolder {
        val textView = LayoutInflater.from(parent.context).inflate(R.layout.news_reader,parent,false)
        return MyViewHolder(textView)
    }

    // 回傳整個 Adapter 包含幾筆資料。
    override fun getItemCount(): Int {
        return newsTitle.size
    }

    // 用來把資料丟入Viewholder中，將其綁定在一起以便Update View
    override fun onBindViewHolder(holder: MyViewHolder, position: Int) {
        holder.newsTitle.text = newsTitle[position]
        holder.newsDescription.text = newsDescription[position]
    }

}