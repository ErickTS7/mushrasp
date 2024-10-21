# **MushRASP**

Min. Android SDK: 28 (Android 9)



# **Features**

> Integrity Validation

> Root Detection

> Emulator Detection

> Frida Detection

> SSL Pinning


## Integrity Validation:

Verify and compares the SHA256 Hash of the key used to sign the APK.


## Root Detection:

Both open source and native functions. Checks for:

su binary

test-keys

root apps (i.e. Magisk)

root cloaking apps

Read-Write /system partition

## Emulator Detection:

Checks device architecture, uptime, hostname, brand name, contacts count and WiFi SSID.


## Frida Detection:

Looks for Frida Server in default port (27042), or in all device ports.


## SSL Pinning

Uses the fullchain.der certificate to validate the host and perform the requisition.




# **Installation**

## 1. Add Jitpack repository to your gradle settings:

```
dependencyResolutionManagement {
	repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
	repositories {
		mavenCentral()
		maven { url 'https://jitpack.io' }
	}
}
```

### 2. Add the RASP dependecy in you gradle build file:

```
dependencies {
    implementation 'com.github.ErickTS7:mushrasp:1.0'
}
```

### 3. Add the necessary permissions into Android Manifest:

```
<uses-permission android:name="android.permission.READ_CONTACTS" />
<uses-permission android:name="android.permission.INTERNET"/>
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"/>
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE"/>
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
```

### 4. Sync gradle files and add the imports:
```
import com.mushroom.rasp.*;
import java.io.InputStream; // For SSL Pinning
```

# **Usage**

## Integrity Validation

First, you need to get the SHA256 Hash of your Key that is being used to sign your APK. It's recommended to use 'keytool', which already comes with Java JDK (check if Java JDK is in you PATH).

```
keytool -list -v -keystore <path_to_keystore> -alias <alias_name> -storepass <store_password>
```

It will return the SHA256 in the format:

```
55:66:77:A5:B2:C3:D4:E5...
```

Copy to a variable in your Main Activity **removing** all **' : '**
It must stay like this:

```
String expectedSignature = "556677A5B2C3D4E5...";
```

Now, call the function to validate the signature:

```
if (!verify.validateAppSignature(this, expectedSignature)) {
    // Do something
}
```

## Root / Emulator / Frida

In order to call any of these detections, you will need to use the "requestPermissions" function:
```
verify.requestPermissions(this, new Runnable() {
@Override
    public void run() {
    
        // CALL THE VERIFICATIONS BELOW !
        
        if (verify.isRooted(MainActivity.this)) {
            // Do something
        }
        if (verify.isRootedNative(MainActivity.this)) {
            // Do something
        }
        if (verify.isEmulated(MainActivity.this)) {
            // Do something
        }
        if (verify.FridaDefaultPort(MainActivity.this)) {
            // Do something
        }
        
        // If you want to use Frida All Ports verification, we recommend to
        // run this function in a new thread due to the long time to finish.
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (verify.FridaAllPorts(MainActivity.this)) {
                    // Do something
                }
            }
        }).start();
        
        // CALL THE VERIFICATIONS ABOVE !
        
    }
});
```

## SSL Pinning

Put the fullchain.der certificate of your domain in the /res/raw directory (if does not exist, create into /app/src/main/res/raw).

Calling the SSL Pinning function:

```
InputStream certInputStream = getResources().openRawResource(R.raw.fullchain);

String response = SslPinningRequest.performRequestWithCert(MainActivity.this, "https://yourdomain.com", certInputStream);
Log.d("SSL Pinning", response);
```

# **Usage Example with all features**

```
// Integrity Validation
String expectedSignature = "815244565E16C601492CD704F242988746550C9BD0862D19684B73B05B1539C9";
if (!verify.validateAppSignature(this, expectedSignature)) {
    Log.e("MainActivity", "[!] Invalid Signature.");
    finishAndRemoveTask();
}

verify.requestPermissions(this, new Runnable() {
    @Override
    public void run() {

        // Root Check
        if (verify.isRooted(MainActivity.this)) {
            Log.e("MushRASP", "[!] Rooted");
            finishAndRemoveTask();
        }

        // Root Check #1
        if (verify.isRootedNative(MainActivity.this)) {
            Log.e("MushRASP", "[!] Rooted-Native");
            finishAndRemoveTask();
        }

        // Root Check #2
        if (verify.isEmulated(MainActivity.this)) {
            Log.e("MushRASP", "[!] Emulated");
            finishAndRemoveTask();
        }

        // Frida Check on default port
        if (verify.FridaDefaultPort(MainActivity.this)) {
            Log.e("MushRASP", "[!] Frida - Default Port");
            finishAndRemoveTask();
        }

        // Frida Check on all ports
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (verify.FridaAllPorts(MainActivity.this)) {
                    Log.e("Verify", "[!] Frida Running");
                    finishAndRemoveTask();
                }
            }
        }).start();

        // SSL Pinning
        InputStream certInputStream = getResources().openRawResource(R.raw.fullchain);
        new Thread(new Runnable() {
            @Override
            public void run() {
                String response = SslPinningRequest.performRequestWithCert(MainActivity.this, "https://example.com", certInputStream);
                Log.d("MushRASP", response);
            }
        }).start();
    }
});

```



