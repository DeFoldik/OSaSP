#ifndef OPENING_H
#define OPENING_H

#include "fileinfo.h"
#include <unistd.h>
#include <sys/wait.h>

void open_selected_files_with_external_programs(FileInfo **file_infos, size_t file_count, int *processed_files, int *selected_files, int selected_count);
const char *get_command_for_mime_type(const char *mime_type);
void view_selected_files_content(FileInfo **file_infos, size_t file_count, int *processed_files);

#endif