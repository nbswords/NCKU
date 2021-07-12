package com.example.qrcode

import android.Manifest.permission.CAMERA
import android.app.AlertDialog
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.provider.Settings.ACTION_APPLICATION_DETAILS_SETTINGS
import android.support.v4.app.ActivityCompat
import android.support.v7.app.AppCompatActivity

class splashActivity : AppCompatActivity() {


        companion object {

            private const val CAMERA_PERMISSION_REQUEST_CODE = 102
        }

        override fun onCreate(savedInstanceState: Bundle?) {
            super.onCreate(savedInstanceState)
            setContentView(R.layout.activity_splash)
            Handler().postDelayed({
                checkForPermission()
            }, 2000)
        }

        private fun checkForPermission() {
            if (ActivityCompat.checkSelfPermission(this, CAMERA) == PackageManager.PERMISSION_GRANTED) {
                gotToMainActivity()
            } else {
                requestPermission()
            }
        }


        private fun requestPermission() {
            ActivityCompat.requestPermissions(
                this, arrayOf(CAMERA),
                CAMERA_PERMISSION_REQUEST_CODE
            )
        }

        private fun gotToMainActivity() {
            startActivity(Intent(this, MainActivity::class.java))
            finish()
        }

        override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
            if (requestCode == CAMERA_PERMISSION_REQUEST_CODE) {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    gotToMainActivity()
                } else if (isPermanentlyDenied()) {
                    showGoToAppSettingsDialog()
                } else
                    requestPermission()
            }
        }

        private fun isPermanentlyDenied(): Boolean {
            return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                shouldShowRequestPermissionRationale(CAMERA).not()
            } else {
                return false
            }
        }


        private fun showGoToAppSettingsDialog() {
            AlertDialog.Builder(this, R.style.CustomAlertDialog)
                .setTitle(getString(R.string.grant_permissions))
                .setMessage(getString(R.string.we_need_permission))
                .setPositiveButton(getString(R.string.grant)) { _, _ ->
                    goToAppSettings()
                }
                .setNegativeButton(getString(R.string.cancel)) { _, _ ->
                    run {
                        finish()
                    }
                }.show()
        }

        private fun goToAppSettings() {
            val intent = Intent(
                ACTION_APPLICATION_DETAILS_SETTINGS,
                Uri.fromParts("package", packageName, null)
            )
            intent.addCategory(Intent.CATEGORY_DEFAULT)
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
            startActivity(intent)
        }

        override fun onRestart() {
            super.onRestart()
            checkForPermission()
        }
}
