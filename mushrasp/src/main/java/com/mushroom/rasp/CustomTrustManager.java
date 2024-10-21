package com.mushroom.rasp;

import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import javax.net.ssl.X509TrustManager;

public class CustomTrustManager implements X509TrustManager {

    private final X509Certificate[] pinnedCertificates;

    public CustomTrustManager(X509Certificate[] pinnedCertificates) {
        this.pinnedCertificates = pinnedCertificates;
    }

    @Override
    public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
        // Não implementado para verificação do cliente
    }

    @Override
    public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
        // Verifica se o certificado do servidor corresponde a um dos certificados fixos (pinned)
        for (X509Certificate pinnedCertificate : pinnedCertificates) {
            if (isCertificateValid(chain[0]) && pinnedCertificate.equals(chain[0])) {
                return; // Certificado válido
            }
        }
        throw new CertificateException("O certificado do servidor não corresponde ao certificado fixado");
    }

    @Override
    public X509Certificate[] getAcceptedIssuers() {
        return new X509Certificate[0];
    }

    // Função que pode ser personalizada para checar a validade de um certificado
    private boolean isCertificateValid(X509Certificate certificate) {
        // Adicionar verificações extras, como expiração ou revogação
        return true;
    }
}