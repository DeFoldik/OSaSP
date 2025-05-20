#ifndef FILEINFO_H
#define FILEINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <magic.h>
#include <openssl/md5.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <pthread.h>
#include <stdio.h>

typedef struct {
    char path[4096];
    char mime_type[128];
    unsigned char md5[MD5_DIGEST_LENGTH];
} FileInfo;

#endif
