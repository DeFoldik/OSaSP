#ifndef FUNC_H
#define FUNC_H

#include "fileinfo.h"


void get_file_mime_type(const char *filepath, char *mime_type, size_t mime_type_len);
void get_file_md5(const char *filepath, unsigned char *md5_digest);
void *process_file(void *arg);

#endif
