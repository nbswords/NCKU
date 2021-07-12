package com.example.qrcode

import android.graphics.Bitmap
import android.graphics.Color
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import com.google.zxing.BarcodeFormat
import com.google.zxing.EncodeHintType
import com.google.zxing.qrcode.QRCodeWriter
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel
import java.util.*

class GenerateQR : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_generate_qr)
//      editText是使用者輸入的網址
        val editText: EditText = findViewById(R.id.value)
//      當按下產生QR Code button的時候  利用輸入的網址產生QR Code
        findViewById<Button>(R.id.btn).setOnClickListener {
            findViewById<ImageView>(R.id.qr).setImageBitmap(generateQrCode(editText.text.toString()))
        }
    }

    private fun generateQrCode(value: String): Bitmap {
//      Init
        val hintMap = Hashtable<EncodeHintType, ErrorCorrectionLevel>()
        hintMap[EncodeHintType.ERROR_CORRECTION] = ErrorCorrectionLevel.H
        val qrCodeWriter = QRCodeWriter() // ZXing提供的QRCodeWriter
        val size = 512
//      產生512*512的QR code
        val bitMatrix = qrCodeWriter.encode(value, BarcodeFormat.QR_CODE, size, size)
        val width = bitMatrix.width
        val bmp = Bitmap.createBitmap(width, width, Bitmap.Config.RGB_565)
//      遍歷QR Code並轉為Bitmap
        for (i in 0 until width) {
            for (j in 0 until width) {
//              If color==Black or Write, Copy pixel back to bitmap
                bmp.setPixel(j, i, if (bitMatrix.get(i, j)) Color.BLACK else Color.WHITE)
            }
        }
        return bmp
    }
}
