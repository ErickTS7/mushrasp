package com.mushroom.rasp;

import android.content.Context;
import java.io.InputStream;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import javax.net.ssl.SSLContext;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class SslPinningRequest {

    // Função para realizar o request com SSL Pinning, usando um certificado fornecido pelo desenvolvedor
    public static OkHttpClient createPinnedClient(InputStream certInputStream) {
        try {
            // Carregar o certificado do servidor fornecido pelo desenvolvedor
            CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
            Certificate serverCertificate = certificateFactory.generateCertificate(certInputStream);

            // Criar um TrustManager com o certificado fixo (pinned)
            X509Certificate[] pinnedCertificates = {(X509Certificate) serverCertificate};
            CustomTrustManager customTrustManager = new CustomTrustManager(pinnedCertificates);

            // Criar um OkHttpClient com SSL pinning
            SSLContext sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, new javax.net.ssl.TrustManager[]{customTrustManager}, null);

            return new OkHttpClient.Builder()
                    .sslSocketFactory(sslContext.getSocketFactory(), customTrustManager)
                    .build();

        } catch (Exception e) {
            e.printStackTrace();
            return new OkHttpClient(); // Voltar para o cliente padrão se houver uma exceção
        }
    }

    // Função para realizar a requisição HTTP usando SSL Pinning, fornecendo o InputStream do certificado
    public static String performRequestWithCert(Context context, String url, InputStream certInputStream) {
        try {
            OkHttpClient client = createPinnedClient(certInputStream);
            Request request = new Request.Builder()
                    .url(url)
                    .build();
            Response response = client.newCall(request).execute();
            return response.isSuccessful() ? response.body().string() : "Erro na requisição";
        } catch (Exception e) {
            e.printStackTrace();
            return "Erro ao realizar a requisição";
        }
    }
}
