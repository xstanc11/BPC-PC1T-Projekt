// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "tariff.h"


int NextTariffID = 0;

/**
 * Initialize Tariff linked list
 * @param list Pointer to tariff list
 */
TariffList_t* TLInit()
{
    TariffList_t* list = malloc(sizeof(TariffList_t));

    if (!list) {
        fprintf(stderr, "Memory allocation for tarif list failed\n");
        exit(-1);
    }

    list->active = NULL;
    list->first = NULL;

    return list;
}

/**
 * Free all allocated nodes
 * @param list Pointer to tariff list
 */
void TLDispose(TariffList_t* list)
{
    TLFirst(list);
    while (list->active) {
        Tariff_t* curr = list->active;
        TLNext(list);
        free(curr);
    }
}

/**
 * Get first tariff
 * @param list Pointer to tariff list
 */
void TLFirst(TariffList_t* list)
{
    list->active = list->first;
}

/**
 * Get next tariff in list
 * @param list Pointer to tariff list
 */
void TLNext(TariffList_t* list)
{
    list->active = list->active->next;
}

/**
 * Returns tariff if name matches
 * @param name Query name
 * @param tariff Pointer to tariff
 */
Tariff_t* TLFindTariffByName(char* name, Tariff_t* tariff)
{
    if (!tariff)
        return NULL;
    if (!strcmp(tariff->name, name))
        return tariff;
    else
        TLFindTariffByName(name, tariff->next);
}

/**
 * Returns tariff if ID matches
 * @param name Query ID
 * @param tariff Pointer to tariff
 */
Tariff_t* TLFindTariffByID(int id, Tariff_t* tariff)
{
    if (!tariff)
        return NULL;
    if (tariff->id == id)
        return tariff;
    else
        TLFindTariffByID(id, tariff->next);
}

/**
 * Insert a tariff into the tariff list
 * Insert new tariffs with id == -1 to ensure correct functioning of TLEdit()
 * @param name Tariff name
 * @param phone Phone number
 * @param list Pointer to tariff list
 */
void TLInsert(int id, char* name, double price, TariffList_t* list)
{
    Tariff_t* prev, * new;
    prev = list->first;

    if (TLFindTariffByName(name, list->first)) {
        printf(RED"Tariff with name %s already exists"RESET"\n", name);
        return;
    }

    new = malloc(sizeof(Tariff_t));
    if (!new) {
        fprintf(stderr, ORANGE"Memory allocation for tariff failed"RESET"\n");
        exit(-1);
    }
    memset(new, 0, sizeof(Tariff_t));

    if (id == -1)
        new->id = NextTariffID++;
    else
        new->id = id;

    strncpy(new->name, name, strlen(name));
    new->price = price;

    TLFirst(list);

    if (!list->active) { // empty list
        new->next = NULL;
        list->first = new;
        list->active = new;
    }
    else if (new->id < list->active->id) { // insert as first
        new->next = list->first;
        list->first = new;
        list->active = new;
    }
    else {
        while (list->active && new->id > list->active->id) {
            prev = list->active;
            TLNext(list);
        }

        prev->next = new;
        new->next = list->active;
        list->active = new;
    }
}

/**
 * Edit a tariff by ID
 * @param id Tariff ID
 * @param name New tariff name (optional)
 * @param price New tariff price (optional, -1 if wanted to be kept original)
 */
void TLEdit(int id, char* name, double price, TariffList_t* list)
{
    Tariff_t* tariff = TLFindTariffByID(id, list->first);
    char newName[MAX_NAME] = { '\0' };
    double newPrice = 0.0;

    if (!tariff) {
        printf(YELLOW"Wrong id (ID = %d), tariff not found"RESET"\n", id);
        return;
    }

    (name[0] != '\0') ? strncpy(newName, name, MAX_NAME - 1) : strncpy(newName, tariff->name, MAX_NAME - 1);
    newName[MAX_NAME - 1] = '\0';
    (price != -1) ? (newPrice = price) : (newPrice = tariff->price);

    TLDelete(tariff->id, list);
    TLInsert(id, newName, newPrice, list);
}

/**
 * Delete a tariff by ID
 * @param id Tariff ID
 * @param list Pointer to tariff list
 */
void TLDelete(int id, TariffList_t* list)
{
    Tariff_t* tariff = TLFindTariffByID(id, list->first);
    if (!tariff) {
        printf(GREEN"Wrong id (ID = %d), tariff not found"RESET"\n", id);
        return;
    }
    TLFirst(list);

    if (!list->active) {
        printf(BLUE"There are no tariffs in the system"RESET"\n");
        return;
    }

    if (list->first == tariff) {
        list->first = tariff->next;
        free(tariff);
        return;
    }

    while (list->active) {
        if (list->active->next == tariff)
            break;
        TLNext(list);
    }

    list->active->next = tariff->next;
    free(tariff);
}

/**
 * Print tariffs
 * @param list Pointer to tariff list
 */
void TLPrint(TariffList_t* list)
{
    TLFirst(list);
    while (list->active) {
        printf(INDIGO"ID: %d\nname: %s\nprice: %.2lfczk"RESET"\n\n", list->active->id, list->active->name, list->active->price);
        TLNext(list);
    }
}