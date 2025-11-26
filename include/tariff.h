// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "main.h"
#include "util.h"

typedef struct Tariff {
    int id;
    char name[MAX_NAME];
    double price;
    struct Tariff *next;
} Tariff_t;

typedef struct {
    Tariff_t *first;
    Tariff_t *active;
} TariffList_t;

TariffList_t* TLInit();
void TLDispose(TariffList_t *);

void TLFirst(TariffList_t *);
void TLNext(TariffList_t *);

Tariff_t* TLFindTariffByName(char *, Tariff_t *);
Tariff_t* TLFindTariffByID(int, Tariff_t *);

void TLInsert(int, char *, double, TariffList_t *);
void TLEdit(int, char *, double, TariffList_t *);
void TLDelete(int, TariffList_t *);

void TLPrint(TariffList_t *);