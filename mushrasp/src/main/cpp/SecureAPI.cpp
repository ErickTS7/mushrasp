#include "SecureAPI.h"
#include <sys/syscall.h>
#include <sys/socket.h> // Adicione essa linha para garantir que o header correto seja incluído

namespace SecureAPI {
    ssize_t write(int fd, const void *buf, size_t count) {
        return ::write(fd, buf, count);
    }

    ssize_t read(int fd, void *buf, size_t count) {
        return ::read(fd, buf, count);
    }

    size_t strlen(const char *s) {
        return ::strlen(s);
    }

    const char* strstr(const char *haystack, const char *needle) {
        return ::strstr(haystack, needle);
    }

    int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
        return ::openat(dirfd, pathname, flags, mode);
    }

    int getdents64(int fd, struct linux_dirent64 *dirp, unsigned int count) {
        return syscall(SYS_getdents64, fd, dirp, count);
    }

    int readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz) {
        return ::readlinkat(dirfd, pathname, buf, bufsiz);
    }

    int close(int fd) {
        return ::close(fd);
    }

    int socket(int domain, int type, int protocol) {
        return ::socket(domain, type, protocol);
    }

    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
        return ::connect(sockfd, addr, addrlen);
    }
}
