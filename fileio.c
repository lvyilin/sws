#include "fileio.h"

void read_file(char *filepath, char *buffer, int max_len) {
    FILE *fp = fopen(filepath, "r");
    int cursor = 0;
    char c;
    if (fp != NULL) {
        while ((c = getc(fp)) != EOF && cursor < max_len) {
            buffer[cursor++] = c;
        }
        buffer[cursor] = '\0';
        fclose(fp);
    }

}

