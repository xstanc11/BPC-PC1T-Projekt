// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "tariff.h"
#include "util.h"

typedef struct Customer {
    int id;
    char name[MAX_NAME];
    char surname[MAX_NAME];
    char phone[MAX_PHONE];
    TariffList_t *assignedTariffs;
    struct Customer *next;
} Customer_t;

typedef struct {
    Customer_t *first;
    Customer_t *active;
} CustomerList_t;

void incrementCustomerId();

CustomerList_t* CLInit();
void CLDispose(CustomerList_t *);

void CLFirst(CustomerList_t *);
void CLNext(CustomerList_t *);

Customer_t* CLFindCustomerByName(char *, char*, Customer_t *);
Customer_t* CLFindCustomerByID(int, Customer_t *);
Customer_t* CLFindCustomerByPhone(char *, Customer_t *);

void CLInsert(int, char *, char *, TariffList_t *, CustomerList_t *);
void CLEdit(int, char *, char *, char *, CustomerList_t *);
void CLDelete(int, CustomerList_t *);

void CLPrint(CustomerList_t *);

void assignTariff(int, int, TariffList_t *, CustomerList_t *);
void unAssignTariff(int, int, TariffList_t *, CustomerList_t *);
void printAssignedTariffs(int, CustomerList_t *);