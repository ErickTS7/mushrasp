#ifndef SECURE_API_H
#define SECURE_API_H

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/socket.h> // Adicione esta linha
#include <arpa/inet.h>
#include <sys/types.h>

namespace SecureAPI {
    ssize_t write(int fd, const void *buf, size_t count);
    ssize_t read(int fd, void *buf, size_t count);
    size_t strlen(const char *s);
    const char* strstr(const char *haystack, const char *needle);
    int openat(int dirfd, const char *pathname, int flags, mode_t mode);
    int getdents64(int fd, struct linux_dirent64 *dirp, unsigned int count);
    int readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);
    int close(int fd);
    int socket(int domain, int type, int protocol);
    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
}

#endif // SECURE_API_H
