package com.mushroom.mushlibrary;

import android.os.Bundle;
import android.util.Log;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import java.io.InputStream;

//import com.mushroom.rasp.*;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        /*


        // #############################################################################################

        // Obter HASH SHA256 da KEY:
        // keytool -list -v -keystore <path_to_keystore> -alias <alias_name> -storepass <store_password>
        // C:\Users\Erickpc\Desktop\keys
        String expectedSignature = "815244565B16C601492CD804F242988747150C9BD0866D19684D73B05B1539C9";

        if (!verify.validateAppSignature(this, expectedSignature)) {
            Log.e("MainActivity", "[!] ASSINATURA INVALIDA.");
            //finishAndRemoveTask();
        }

        verify.requestPermissions(this, new Runnable() {
            @Override
            public void run() {
                if (verify.isRooted(MainActivity.this)) {
                    Log.e("TAG", "[!] Rooted");
                }
                if (verify.isRootedNative(MainActivity.this)) {
                    Log.e("TAG", "[!] Rooted-Native");
                }
                if (verify.isEmulated(MainActivity.this)) {
                    Log.e("TAG", "[!] EMULATED");
                }
                if (verify.FridaDefaultPort(MainActivity.this)){
                    Log.e("TAG", "[!] Frida - Default Port");
                }

                // Executa o FridaAllPorts em uma nova Thread
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (verify.FridaAllPorts(MainActivity.this)) {
                            Log.e("Verify", "[!] FRIDA-ALL-PORTS!");
                        }
                    }
                }).start();

                //############################################################# SSL PINNING
                InputStream certInputStream = getResources().openRawResource(R.raw.fullchain);

                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String response = SslPinningRequest.performRequestWithCert(MainActivity.this, "https://novakrc.xyz", certInputStream);
                        Log.d("SSL Pinning", response);
                    }
                }).start();

                //#############################################################

            }
        });

        */

    }
}