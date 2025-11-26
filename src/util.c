// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "main.h"
#include "util.h"

/**
 * Flush stdin after scanf()
 */
void flushStdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * Reads from stdin and stores in buffer
 * @param buff Pointer to buffer
 */
void readLine(char *buff)
{
    if (fgets(buff, sizeof(buff), stdin) == NULL) {
        buff[0] = '\0';
        return;
    }
    size_t len = strlen(buff);
    if (len > 0 && buff[len-1] == '\n')
        buff[len-1] = '\0';
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