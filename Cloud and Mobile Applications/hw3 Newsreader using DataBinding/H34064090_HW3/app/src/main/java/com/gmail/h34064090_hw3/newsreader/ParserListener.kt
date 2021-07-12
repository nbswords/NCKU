package com.gmail.h34064090_hw3.newsreader

interface ParserListener {
    fun start()
    fun finish()
    fun setNewData(new: NewsData)

}