// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "tariff.h"
#include "customer.h"

enum PATHS {
    LINUX,
    WINDOWS
};

void parseTariffs(char *, int, TariffList_t *, CustomerList_t *);

void readFile(int, TariffList_t *, CustomerList_t *);
void saveFile(int, TariffList_t *, CustomerList_t *);
