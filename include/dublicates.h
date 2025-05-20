#ifndef DUPLICATE_FINDER_H
#define DUPLICATE_FINDER_H

#include "fileinfo.h"
#include "opening.h"

int find_duplicate_files(FileInfo **file_infos, size_t file_count, int *processed_files);
void delete_duplicates(int duplicate_counter, FileInfo **file_infos, size_t file_count, int *processed_files);

#endif