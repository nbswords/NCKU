package com.gmail.h34064090_hw3.newsreader

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.databinding.DataBindingUtil
import com.gmail.h34064090_hw3.newsreader.databinding.PreviewBinding

class PreviewActivity : AppCompatActivity() {

    var news = mutableListOf<NewsData>()
        private lateinit var binding: PreviewBinding

        override fun onCreate(savedInstanceState: Bundle?) {
            super.onCreate(savedInstanceState)
            binding = DataBindingUtil.setContentView(this, R.layout.preview)

            binding.title = intent.getStringExtra("title")
            binding.description = intent.getStringExtra("description")

        }

    }