// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "tariff.h"
#include "customer.h"

void parseTariffs(char *, int, TariffList_t *, CustomerList_t *);

void readFile(TariffList_t *, CustomerList_t *);
void saveFile(TariffList_t *, CustomerList_t *);
