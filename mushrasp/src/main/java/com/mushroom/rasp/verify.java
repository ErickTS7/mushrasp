package com.mushroom.rasp;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.widget.Toast;
import com.karumi.dexter.Dexter;
import com.karumi.dexter.MultiplePermissionsReport;
import com.karumi.dexter.PermissionToken;
import com.karumi.dexter.listener.PermissionRequest;
import com.karumi.dexter.listener.multi.MultiplePermissionsListener;

import android.content.pm.PackageManager;
import android.os.Build;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.List;

public class verify {

    static {
        System.loadLibrary("mush-lib"); // Carrega a biblioteca nativa
    }

    // Métodos nativos
    private static native boolean isEmulatedN(Context context);
    private static native boolean isRootedN(Context context);
    private static native boolean isDeviceRootedN(Context context);
    private static native boolean detectFridaDefaultPort(Context context);
    private static native boolean detectFridaAllPorts(Context context);
    private static native boolean validateAppSignatureN(Context context, String expectedSignature);




    // Funções para chamar os métodos nativos
    public static boolean isEmulated(Context context) {
        return isEmulatedN(context);
    }

    public static boolean isRooted(Context context) { return isRootedN(context); }

    public static boolean isRootedNative(Context context) {
        return isDeviceRootedN(context);
    }

    public static boolean FridaDefaultPort(Context context) { return detectFridaDefaultPort(context); }

    public static boolean FridaAllPorts(Context context) {
        return detectFridaAllPorts(context);
    }



    // Função pública que pode ser chamada pela MainActivity
    public static boolean validateAppSignature(Context context, String expectedSignature) {
        return validateAppSignatureN(context, expectedSignature); // Chama o método nativo corretamente
    }


    // Função para solicitar permissões
    public static void requestPermissions(Activity context, final Runnable onPermissionsResult) {
        Dexter.withActivity(context)
                .withPermissions(
                        Manifest.permission.READ_CONTACTS,
                        Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.ACCESS_WIFI_STATE,
                        Manifest.permission.ACCESS_NETWORK_STATE,
                        Manifest.permission.INTERNET
                )
                .withListener(new MultiplePermissionsListener() {
                    @Override
                    public void onPermissionsChecked(MultiplePermissionsReport report) {
                        if (report.areAllPermissionsGranted()) {
                            onPermissionsResult.run();
                        } else {
                            Toast.makeText(context, "Permissões necessárias não foram concedidas", Toast.LENGTH_SHORT).show();
                            context.finishAndRemoveTask();
                        }
                    }

                    @Override
                    public void onPermissionRationaleShouldBeShown(List<PermissionRequest> permissions, PermissionToken token) {
                        token.continuePermissionRequest();
                    }
                })
                .withErrorListener(error -> {
                    Toast.makeText(context.getApplicationContext(), "Error occurred! ", Toast.LENGTH_SHORT).show();
                    context.finishAndRemoveTask();
                })
                .onSameThread()
                .check();
    }

}
