#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

typedef struct FileInfo {
    char name[256];
    time_t mod_time;
} FileInfo;

void check_directory(const char *path, FileInfo *files, int *file_count) {
    struct dirent *entry;
    struct stat file_stat;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISREG(file_stat.st_mode)) {
            int found = 0;
            for (int i = 0; i < *file_count; i++) {
                if (strcmp(files[i].name, entry->d_name) == 0) {
                    found = 1;
                    if (files[i].mod_time != file_stat.st_mtime) {
                        printf("File %s modified.\n", entry->d_name);
                        files[i].mod_time = file_stat.st_mtime;
                    }
                    break;
                }
            }
            if (!found) {
                printf("File %s created.\n", entry->d_name);
                strcpy(files[*file_count].name, entry->d_name);
                files[*file_count].mod_time = file_stat.st_mtime;
                (*file_count)++;
            }
        }
    }

    for (int i = 0; i < *file_count; i++) {
        int found = 0;
        rewinddir(dp);
        while ((entry = readdir(dp)) != NULL) {
            if (strcmp(files[i].name, entry->d_name) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("File %s deleted.\n", files[i].name);
            for (int j = i; j < *file_count - 1; j++) {
                files[j] = files[j + 1];
            }
            (*file_count)--;
            i--;
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s PATH\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *path = argv[1];
    FileInfo files[1024];
    int file_count = 0;

    while (1) {
        check_directory(path, files, &file_count);
        sleep(5); // 每隔5秒檢查一次
    }

    return 0;
}
