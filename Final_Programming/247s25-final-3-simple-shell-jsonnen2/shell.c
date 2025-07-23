
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (true) {
        printf("$ ");
        fflush(stdout);
        read = getline(&line, &len, stdin);

        // Detect end of file
        if (read == -1) {
            break;
        }

        // Remove trailing newline
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        if (strcmp(line, "ls") == 0) {
            // no args ls command
            DIR *d = opendir(".");
            if (!d) {
                perror("opendir");
                continue;
            }

            struct dirent *entry;
            while ((entry = readdir(d)) != NULL) {
                if (entry->d_type == DT_DIR) {
                    printf("DIR %s\n", entry->d_name);
                } else if (entry->d_type == DT_REG) {
                    printf("REG %s\n", entry->d_name);
                }
                // Other types can be ignored for now
            }

            closedir(d);
        } else if (strstr(line, "cd")) {

        }
        // Unrecognized command
        else {
            printf("Unknown command: %s\n", line);
        }
    }

    free(line);
    return 0;
}
