package com.gmail.h34064090_hw3.newsreader

import android.util.Log
import org.xml.sax.Attributes
import org.xml.sax.helpers.DefaultHandler
import java.net.URL
import java.util.concurrent.Executors
import javax.xml.parsers.SAXParserFactory

class NewsSAX (private val listener:ParserListener): DefaultHandler() {
    private val factory = SAXParserFactory.newInstance()
    private val parser = factory.newSAXParser()

    private var entryFound = false
    private var titleFound = false
    private var descriptionFound = false
    private var element: String = ""

    private var Title = ""
    private var newsDescription = ""

    private var service = Executors.newFixedThreadPool(1)

    override fun characters(ch: CharArray?, start: Int, length: Int) {
        super.characters(ch, start, length)
        ch?.let {
            element += String(it, start, length)
        }
    }


    override fun endElement(uri: String?, localName: String?, qName: String?) {
        super.endElement(uri, localName, qName)

        if (entryFound) {
            if (titleFound) {
                Title = element
                titleFound = false
            }
            else if (descriptionFound) {
                entryFound = false
                newsDescription = element
                descriptionFound = false

                Log.i("Title", Title)
                Log.i("Description", newsDescription)

                service.execute {
                    listener.setNewData(NewsData(Title, newsDescription))
                }


            }
        }
        if (localName == "item") {
            entryFound = false
        }


    }

    override fun startElement(uri: String?, localName: String?, qName: String?, attributes: Attributes?) {
        super.startElement(uri, localName, qName, attributes)
        if (localName == "item") {
            entryFound = true
        }
        if (entryFound) {
            if (localName == "title") {
                titleFound = true}
                if (localName == "description") {
                    descriptionFound = true
            }
        }

        element = ""
    }

    override fun endDocument() {
        super.endDocument()
        listener.finish()
    }

    override fun startDocument() {
        super.startDocument()
        listener.start()
    }



    fun parseURL(url: String) {
        Thread(Runnable {
            try {
                val inputStream = URL(url).openStream()
                parser.parse(inputStream, this@NewsSAX)
            } catch (e: Throwable) {
                e.printStackTrace()
            }
        }).start()
    }
}
