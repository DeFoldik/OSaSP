#include "func.h"

void get_file_mime_type(const char *filepath, char *mime_type, size_t mime_type_len) {
    magic_t cookie = magic_open(MAGIC_MIME_TYPE);
    if (cookie == NULL) {
        strncpy(mime_type, "application/octet-stream", mime_type_len);
        return;
    }

    if (magic_load(cookie, NULL) == -1 || magic_compile(cookie, NULL) == -1) {
        strncpy(mime_type, "application/octet-stream", mime_type_len);
        magic_close(cookie);
        return;
    }

    const char *result = magic_file(cookie, filepath);
    if (result == NULL) {
        strncpy(mime_type, "application/octet-stream", mime_type_len);
    } else {
        strncpy(mime_type, result, mime_type_len);
    }

    magic_close(cookie);
}

void get_file_md5(const char *filepath, unsigned char *md5_digest) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return;
    }

    unsigned char buffer[4096];
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit(ctx, EVP_md5());

    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        EVP_DigestUpdate(ctx, buffer, bytes_read);
    }

    if (bytes_read == -1) {
        perror("read");
        close(fd);
        return;
    }

    unsigned int md_len;
    EVP_DigestFinal(ctx, md5_digest, &md_len);
    EVP_MD_CTX_free(ctx);
    close(fd);
}

void *process_file(void *arg) {
    char *filepath = (char *) arg;
    FileInfo *file_info = malloc(sizeof(FileInfo));

    strncpy(file_info->path, filepath, sizeof(file_info->path));
    get_file_mime_type(filepath, file_info->mime_type, sizeof(file_info->mime_type));
    get_file_md5(filepath, file_info->md5);

    return file_info;
}
