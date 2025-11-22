// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "main.h"

void flush_stdin(void);

void read_line(const char *prompt, char *buf, size_t sz);

void splitName(char *, char *, char *);