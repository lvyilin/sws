#include "fileio.h"
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>

void read_file(char *filepath, char *dest, int max_len) {
    FILE *fp = fopen(filepath, "r");
    int cursor = 0;
    char c;
    if (fp != NULL) {
        while ((c = getc(fp)) != EOF && cursor < max_len - 1) {
            dest[cursor++] = c;
        }
        dest[cursor] = '\0';
        fclose(fp);
    }

}

int is_absolute_path(char *filepath) {
    if (filepath != NULL && (filepath[0] == '/')) {
        return 1;
    }
    return 0;
}

void get_absolute_path(char *relative_path, char *absolute_path) {
    if (!is_absolute_path(relative_path)) {
        char buffer[1024];
        if (relative_path[0] == '~') {
            struct passwd *pw = getpwuid(getuid());
            const char *homedir = pw->pw_dir;
            strcpy(buffer, homedir);
            strcat(buffer, "/");
            strcat(buffer, &relative_path[1]);

        } else {
            getcwd(buffer, sizeof(buffer));
            strcat(buffer, "/");
            strcat(buffer, relative_path);
        }

        realpath(buffer, absolute_path);
//        strcpy(absolute_path,buffer);

    }
}