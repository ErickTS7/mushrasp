#include "FridaDetect.h"
#include "SecureAPI.h"
#include "Log.h"
#include <fcntl.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstdio>
#include <unistd.h>  // Para sleep e close

#define TIMEOUT 3 // Timeout de 3 segundos para operações de socket

extern "C" JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_detectFridaAllPorts(JNIEnv *, jclass, jobject context) {
    return FridaDetect::detectFridaAllPorts();
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_detectFridaDefaultPort(JNIEnv *, jclass, jobject context) {
    return FridaDetect::detectFridaDefaultPort();
}

bool FridaDetect::detectFridaAllPorts() {
    LOGI("FridaDetect: Scanning All Ports");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    for (int i = 1; i < 65536; i++) {
        int fd = SecureAPI::socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
            LOGE("FridaDetect::detectFridaListener socket failed, errno: %d", errno);
            continue;  // Pular para a próxima porta se não for possível criar o socket
        }

        // Definir timeout para o socket
        struct timeval timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
        setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

        addr.sin_port = htons(i);

        if (SecureAPI::connect(fd, (const struct sockaddr *) &addr, sizeof(addr)) == 0) {
            char req[1024];
            sprintf(req, "GET /ws HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: CpxD2C5REVLHvsUC9YAoqg==\r\nSec-WebSocket-Version: 13\r\nHost: %s:%d\r\nUser-Agent: Frida/16.1.7\r\n\r\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
            SecureAPI::write(fd, req, SecureAPI::strlen(req));

            char res[1024];
            int read_res = SecureAPI::read(fd, res, sizeof(res));

            if (read_res > 0) {
                if (SecureAPI::strstr(res, "tyZql/Y8dNFFyopTrHadWzvbvRs=")) {
                    LOGI("Frida Server fingerprint found on port %d", ntohs(addr.sin_port));
                    close(fd);
                    return true;
                }
            }
        }

        close(fd);  // Garantir que o socket seja sempre fechado
        //LOGI("Scanning port %d", ntohs(addr.sin_port));

        // Pausa após cada intervalo de portas para evitar sobrecarga
        if (i == 20000 || i == 40000 || i == 60000) {
            sleep(1);
        }
    }

    return false;
}


bool FridaDetect::detectFridaDefaultPort() {
    LOGI("FridaDetect: Scanning Default Port");

    int fd = SecureAPI::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        LOGE("FridaDetect::detectFridaOnPort27042 socket failed, errno: %d", errno);
        return true;
    }

    // Definir timeout para o socket
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(27042);  // Verificando a porta 27042

    if (SecureAPI::connect(fd, (const struct sockaddr *) &addr, sizeof(addr)) == 0) {
        char req[1024];
        sprintf(req, "GET /ws HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: CpxD2C5REVLHvsUC9YAoqg==\r\nSec-WebSocket-Version: 13\r\nHost: %s:%d\r\nUser-Agent: Frida/16.1.7\r\n\r\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        SecureAPI::write(fd, req, SecureAPI::strlen(req));

        char res[1024];
        if (SecureAPI::read(fd, res, sizeof(res)) > 0) {
            if (SecureAPI::strstr(res, "tyZql/Y8dNFFyopTrHadWzvbvRs=")) {
                LOGI("Frida Server fingerprint found on port 27042");
                close(fd);
                return true;
            }
        }
    }

    LOGI("Scanning default port 27042");
    close(fd);

    return false;
}
