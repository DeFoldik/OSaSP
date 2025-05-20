#include "dublicates.h"

int find_duplicate_files(FileInfo **file_infos, size_t file_count, int *processed_files) {
    printf("Duplicate files:\n");
    int duplicate_counter = 0;

    for (size_t i = 0; i < file_count; i++) {
        if (processed_files[i]) continue;

        FileInfo *current_file = file_infos[i];
        int has_duplicates = 0;

        for (size_t j = i + 1; j < file_count; j++) {
            if (processed_files[j]) continue;

            FileInfo *compare_file = file_infos[j];
            if (memcmp(current_file->md5, compare_file->md5, MD5_DIGEST_LENGTH) == 0 && strcmp(current_file->mime_type, compare_file->mime_type) == 0) {
                if (!has_duplicates) {
                    printf("Duplicate found for file: %s\n", current_file->path);
                    has_duplicates = 1;
                }
                duplicate_counter++;
                printf("Duplicate %d: %s\n", duplicate_counter, compare_file->path);
                processed_files[j] = 1;
            }
        }

        if (has_duplicates) {
            processed_files[i] = 1;
        }
    }

    return duplicate_counter;
}

void delete_duplicates(int duplicate_counter, FileInfo **file_infos, size_t file_count, int *processed_files) {
    if (duplicate_counter > 0) {
        view_selected_files_content(file_infos, file_count, processed_files);
        char user_input[1024];
        printf("Do you want to delete the found duplicates? (y/n) ");
        fgets(user_input, sizeof(user_input), stdin);

        if (user_input[0] == 'y' || user_input[0] == 'Y') {
            printf("Enter the paths of the duplicates you want to delete, separated by commas: ");
            fgets(user_input, sizeof(user_input), stdin);

            // Удаляем символ новой строки '\n' в конце ввода
            user_input[strcspn(user_input, "\n")] = '\0';

            char *token = strtok(user_input, ",");
            while (token != NULL) {
                char *path = token;
                FileInfo *file_to_delete = NULL;

                if (*path != '\0') {
                    for (size_t i = 0; i < file_count; i++) {
                        if (!processed_files[i]) continue;

                        FileInfo *file_info = file_infos[i];
                        if (strcmp(file_info->path, path) == 0) {
                            file_to_delete = file_info;
                            break;
                        }
                    }

                    if (file_to_delete != NULL) {
                        if (unlink(file_to_delete->path) != 0) {
                            perror("Error deleting file");
                        } else {
                            printf("Deleted file: %s\n", file_to_delete->path);
                        }
                    } else {
                        printf("Invalid file path: %s\n", path);
                    }
                }

                token = strtok(NULL, ",");
            }
        }
    }
}
