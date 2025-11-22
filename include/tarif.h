// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "main.h" // max size definitions

typedef struct Tarif {
    int id;
    char name[MAX_NAME];
    double price;
    struct Tarif *next;
} Tarif_t;

typedef struct {
    Tarif_t *first;
    Tarif_t *active;
} TarifList_t;

void TLInit(TarifList_t *);
void TLDispose(TarifList_t *);

void TLFirst(TarifList_t *);
void TLLast(TarifList_t *);
void TLNext(TarifList_t *);
Tarif_t* TLFindTarifByName(char *, Tarif_t *);
Tarif_t* TLFindTarifByID(int, Tarif_t *);

void TLInsert(int, char *, double, TarifList_t *);
void TLEdit(int, char *, double, TarifList_t *);
void TLDelete(int, TarifList_t *);
void TLPrint(TarifList_t *);