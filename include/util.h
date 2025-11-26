// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#define MAX_NAME 100
#define MAX_PHONE 32
#define LINE_BUF 256

#define RED     "\033[31m"
#define ORANGE  "\033[33m"
#define YELLOW  "\033[93m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define INDIGO  "\033[35m"
#define VIOLET  "\033[95m"
#define RESET   "\033[0m"

void flush_stdin();

void read_line(const char *, char *, size_t);

void splitName(char *, char *, char *);