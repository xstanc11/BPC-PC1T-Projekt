// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include "util.h"

void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_line(const char *prompt, char *buf, size_t sz)
{
    if (prompt)
        printf("%s", prompt);
    if (fgets(buf, (int)sz, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t ln = strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n')
        buf[ln-1] = '\0';
}
