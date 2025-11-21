// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_PHONE 32
#define LINE_BUF 256

/* Strukturka pro Tarif (globální seznam tarifů) */
typedef struct Tarif {
    int id;
    char name[MAX_NAME];
    double price;
    struct Tarif *next;
} Tarif;

/* Pro každý zákazník: seznam přiřazených tarifů (ukazatele na Tarif) */
typedef struct CustTarifNode {
    Tarif *tarif; /* ukazatel na tarif v globálním seznamu */
    struct CustTarifNode *next;
} CustTarifNode;

/* Rodinný tarif: kolekce zákazníků sdílející tarif (bonus) */
typedef struct FamilyPlan {
    int id;
    char name[MAX_NAME]; /* název rodinného plánu */
    Tarif *tarif; /* tarif, který sdílí */
    // Customer **members; /* dynamické pole ukazatelů na členy (jednodušší implementace) */
    int member_count;
    struct FamilyPlan *next;
} FamilyPlan;
