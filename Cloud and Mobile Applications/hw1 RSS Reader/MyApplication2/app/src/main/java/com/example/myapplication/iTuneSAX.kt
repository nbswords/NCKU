package com.example.myapplication

import org.xml.sax.Attributes
import org.xml.sax.helpers.DefaultHandler
import java.net.URL
import javax.xml.parsers.SAXParserFactory

class iTuneSAX (val listener: ParserListener): DefaultHandler(){
    private val factory = SAXParserFactory.newInstance()
    private val parser = factory.newSAXParser()

    private var itemFound = false
    private var titleFound = false
    private var element: String = ""

    fun parseURL(url: String){
        Thread(Runnable{
            try{
               val inputStream = URL(url).openStream()
               parser.parse(inputStream, this@iTuneSAX)
            } catch (e: Throwable){
                e.printStackTrace()
            }
        }).start()
    }

    override fun endElement(uri: String?, localName: String?, qName: String?) {
        super.endElement(uri, localName, qName)
        if (itemFound){
            if(titleFound){
                listener.setTitle(element)
                titleFound = false
            }
        }

        if (localName == "item"){
            itemFound = false
        }
    }

    // parse
    override fun characters(ch: CharArray?, start: Int, length: Int) {
        super.characters(ch, start, length)
        ch?.let {
            element += String(it, start, length)
        }
    }


    // entry receive title
    override fun startElement(
        uri: String?,
        localName: String?,
        qName: String?,
        attributes: Attributes?
    ) {
        super.startElement(uri, localName, qName, attributes)
        if (localName == "item") {
            itemFound = true
        }
        if (itemFound) {
            if (localName == "title") {
                titleFound = true
            }
        }
        element = ""
    }
}