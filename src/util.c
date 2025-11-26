// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "main.h"
#include "util.h"

void flushStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void readLine(const char *prompt, char *buf, size_t sz)
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
/**
 * Splits a string consisting of a name and surname into two
 * @param fullName Full name separated by a space
 * @param name Pointer to variable for the name
 * @param name Pointer to variable for the surname
 */
void splitName(char *fullName, char *name, char *surname)
{
    char tmp[2 * MAX_NAME] = {'\0'};
    strncpy(tmp, fullName, sizeof(tmp));
    tmp[2 * MAX_NAME - 1] = '\0';
    strncpy(name, strtok(tmp, " "), MAX_NAME - 1);
    name[MAX_NAME - 1] = '\0';
    strncpy(surname, strtok(NULL, " "), MAX_NAME - 1);
    surname[MAX_NAME - 1] = '\0';
}