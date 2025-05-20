#include "opening.h"

void open_selected_files_with_external_programs(FileInfo **file_infos, size_t file_count, int *processed_files, int *selected_files, int selected_count) {
    if (selected_count == 0) {
        return;
    }

    for (int i = 0; i < selected_count; i++) {
        int index = selected_files[i];
        if (!processed_files[index]) continue;
        FileInfo *file_info = file_infos[index];

        char user_input[1024];
        printf("Do you want to view the contents of the file '%s'? (y/n) ", file_info->path);
        fgets(user_input, sizeof(user_input), stdin);

        // Check for invalid input
        while (user_input[0] != 'y' && user_input[0] != 'Y' && user_input[0] != 'n' && user_input[0] != 'N') {
            printf("Invalid input. Please enter 'y' or 'n': ");
            fgets(user_input, sizeof(user_input), stdin);
        }

        if (user_input[0] == 'y' || user_input[0] == 'Y') {
            const char *command = get_command_for_mime_type(file_info->mime_type);
            if (command != NULL) {
                const char* args[] = {command, file_info->path, NULL};
                pid_t pid = fork();

                if (pid == 0) {
                    execvp(command, (char *const *)args);
                    perror("execvp failed");
                    exit(1);
                } else if (pid > 0) {
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("fork failed");
                }
            } else {
                printf("No suitable program found for file: %s\n", file_info->path);
            }
        }
    }
}

void view_selected_files_content(FileInfo **file_infos, size_t file_count, int *processed_files) {
    char user_input[1024];
    printf("Do you want to view the contents of specific files? (y/n) ");
    fgets(user_input, sizeof(user_input), stdin);

    // Check for invalid input
    while (user_input[0] != 'y' && user_input[0] != 'Y' && user_input[0] != 'n' && user_input[0] != 'N') {
        printf("Invalid input. Please enter 'y' or 'n': ");
        fgets(user_input, sizeof(user_input), stdin);
    }

    if (user_input[0] == 'y' || user_input[0] == 'Y') {
        printf("Enter the paths of the files you want to view, separated by commas: ");
        fgets(user_input, sizeof(user_input), stdin);

        // Удаляем символ новой строки '\n' в конце ввода
        user_input[strcspn(user_input, "\n")] = '\0';

        char *token = strtok(user_input, ",");
        while (token != NULL) {
            char *path = token;
            int index = -1;

            if (*path != '\0') {
                for (size_t i = 0; i < file_count; i++) {
                    if (processed_files[i] && strcmp(file_infos[i]->path, path) == 0) {
                        index = i;
                        break;
                    }
                }

                if (index >= 0) {
                    int *selected_files = malloc(sizeof(int));
                    selected_files[0] = index;
                    open_selected_files_with_external_programs(file_infos, file_count, processed_files, selected_files, 1);
                    free(selected_files);
                } else {
                    printf("Invalid file path: %s\n", path);
                }
            }

            token = strtok(NULL, ",");
        }
    }
}


const char *get_command_for_mime_type(const char *mime_type) {

    const char *commands[][2] = {
         {"image/jpeg", "/usr/bin/eog"},
         {"image/png", "/usr/bin/eog"},
         {"text/plain", "/usr/bin/nano"},
         {"application/pdf", "/usr/bin/evince"}
    };

    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(mime_type, commands[i][0]) == 0) {
            return commands[i][1];
        }
    }

    return NULL;
}

