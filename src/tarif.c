// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include "tarif.h"

int NextTarifID = 0;

/**
 * Initialize Tarif linked list
 * @param list Pointer to tarif list
 */
void TLInit(TarifList_t *list)
{
    list->active = NULL;
    list->first = NULL;
}

/**
 * Free all allocated nodes
 * @param list Pointer to tarif list
 */
void TLDispose(TarifList_t *list)
{
    TLFirst(list);
    while (list->active) {
        Tarif_t *curr = list->active;
        TLNext(list);
        free(curr);
    }
}

/**
 * Get first tarif
 * @param list Pointer to tarif list
 */
void TLFirst(TarifList_t *list)
{
    list->active = list->first;
}

/**
 * Get last tarif
 * @param list Pointer to tarif list
 */
void TLLast(TarifList_t *list)
{
    TLFirst(list);
    while (list->active) {
        if (list->active->next == NULL)
            return;
        TLNext(list);
    }
}

/**
 * Get next tarif in list
 * @param list Pointer to tarif list
 */
void TLNext(TarifList_t *list)
{
    list->active = list->active->next;
}

/**
 * Returns tarif if name matches
 * @param name Query name
 * @param tarif Pointer to tarif
 */
Tarif_t* TLFindTarifByName(char *name, Tarif_t *tarif)
{
    if (!tarif)
        return NULL;
    if (!strcmp(tarif->name, name))
        return tarif;
    else
        TLFindTarifByName(name, tarif->next);
}

/**
 * Returns tarif if ID matches
 * @param name Query ID
 * @param tarif Pointer to tarif
 */
Tarif_t* TLFindTarifByID(int id, Tarif_t *tarif)
{
    if (!tarif)
        return NULL;
    if (tarif->id == id)
        return tarif;
    else
        TLFindTarifByID(id, tarif->next);
}

/**
 * Insert a tarif into the tarif list
 * Insert new tarifs with id == -1 to ensure correct functioning of TLEdit()
 * @param name Tarif name
 * @param phone Phone number
 * @param list Pointer to tarif list
 */
void TLInsert(int id, char *name, double price, TarifList_t *list)
{
    Tarif_t *prev, *new;
    prev = list->first;

    if (TLFindTarifByName(name, list->first)) {
        printf("Tarif with name %s already exists\n", name);
        return;
    }

    new = malloc(sizeof(Tarif_t));
    if (!new) {
        fprintf(stderr, "Memory allocation for tarif failed\n");
        exit(-1);
    }
    memset(new, 0, sizeof(Tarif_t));

    if (id == -1)
        new->id = NextTarifID++;
    else
        new->id = id;

    strncpy(new->name, name, strlen(name));
    new->price = price;

    TLFirst(list);

    if (!list->active) { // empty list
        new->next = NULL;
        list->first = new;
        list->active = new;
    } else if (new->id < list->active->id) { // insert as first
        new->next = list->first;
        list->first = new;
        list->active = new;
    } else {
        while (list->active &&  new->id > list->active->id) {
            prev = list->active;
            TLNext(list);
        }
        
        prev->next = new;
        new->next = list->active;
        list->active = new;
    }
}

/**
 * Edit a tarif by ID
 * @param id Tarif ID
 * @param name New tarif name (optional)
 * @param price New tarif price (optional, -1 if wanted to be kept original) 
 */
void TLEdit(int id, char *name, double price, TarifList_t *list)
{
    Tarif_t *tarif = TLFindTarifByID(id, list->first);
    char newName[MAX_NAME] = {'\0'};
    double newPrice = 0.0;

    if (!tarif) {
        printf("Wrong id (ID = %d), tarif not found\n", id);
        return;
    }

    (name) ? strncpy(newName, name, MAX_NAME - 1) : strncpy(newName, tarif->name, MAX_NAME - 1);
    newName[MAX_NAME - 1] = '\0';
    (price != -1) ? (newPrice = price) : (newPrice = tarif->price);

    TLDelete(tarif->id, list);
    TLInsert(id, newName, newPrice, list);
}

/**
 * Delete a tarif by ID
 * @param id Tarif ID
 * @param list Pointer to tarif list
 */
void TLDelete(int id, TarifList_t *list)
{
    Tarif_t *tarif = TLFindTarifByID(id, list->first);
    if (!tarif) {
        printf("Wrong id (ID = %d), tarif not found\n", id);
        return;
    }
    TLFirst(list);

    if (!list->active) {
        printf("There are no tarifs in the system\n");
        return;
    }
    
    if (list->first == tarif) {
        list->first = tarif->next;
        free(tarif);
        return;
    }

    while (list->active) {
        if (list->active->next == tarif)
            break;
        TLNext(list);
    }

    list->active->next = tarif->next;
    free(tarif);
}

/**
 * Print tarifs
 * @param list Pointer to tarif list
 */
void TLPrint(TarifList_t *list)
{
    TLFirst(list);
    while (list->active) {
        printf("ID: %d\nname: %s\nprice: %lf\n", list->active->id, list->active->name, list->active->price);
        TLNext(list);
    }
}