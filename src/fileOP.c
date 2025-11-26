// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileOP.h"

/**
 * Parses out tariffs during file read and assigns them correctly
 * @param tariffs Pointer to list of tariffs to be assigned
 * @param id Id of customer
 * @param tariffList Pointer to list of tariffs
 *  @param customerList Pointer to list of customers
 */
void parseTariffs(char *tariffs, int id, TariffList_t *tariffList, CustomerList_t *customerList)
{
    char *token, tariffId[1];
    if (tariffs[0] == '-')
        return;

    token = strtok(tariffs, ",");
    while (token != NULL) {
        assignTariff(atoi(token), id, tariffList, customerList);
        token = strtok(NULL, ",");
    }
}

/**
 * Reads from persistent file into memory
 * @param tariffList Pointer to list of tariffs
 * @param customerList Pointer to list of customers
 */
void readFile(int machine, TariffList_t *tariffList, CustomerList_t *customerList)
{
    FILE *f;
    char inputBuff[MAX_NAME * 4] = {'\0'}; // for good measure
    char *id, name[MAX_NAME], phone[MAX_PHONE], price[MAX_NAME], tariffs[MAX_NAME];
    char c;
    char *walker = inputBuff;

    if (machine == LINUX)
        f = fopen("../files/tariff.txt", "r");
    else
        f = fopen("../../../files/tariff.txt", "r");
    
    if (!f) {
        fprintf(stderr, RED"Unable to open tariff.txt\n"RESET);
        exit(-1);
    }

    while ((c = (char)fgetc(f)) != EOF) {
        if (c == '\n') {
            id = strtok(inputBuff, ";");
            strncpy(name, strtok(NULL, ";"), sizeof(name));
            strncpy(price, strtok(NULL, ";"), sizeof(price));

            TLInsert(atoi(id), name, atol(price), tariffList);

            memset(inputBuff, '\0', sizeof(inputBuff));
            walker = inputBuff;
        }
        *walker = c;
        walker++;
    }

    fclose(f);

    if (machine == LINUX)
        f = fopen("../files/customer.txt", "r");
    else
        f = fopen("../../../files/customer.txt", "r");


    if (!f) {
        fprintf(stderr, RED"Unable to open customer.txt\n"RESET);
        exit(-1);
    }

    while ((c = (char)fgetc(f)) != EOF) {
        if (c == '\n') {
            id = strtok(inputBuff, ";");
            strncpy(name, strtok(NULL, ";"), sizeof(name));
            strncpy(phone, strtok(NULL, ";"), sizeof(phone));
            strncpy(tariffs, strtok(NULL, ";"), sizeof(tariffs));

            CLInsert(atoi(id), name, phone, NULL, customerList);
            parseTariffs(tariffs, atoi(id), tariffList, customerList);

            memset(inputBuff, '\0', sizeof(inputBuff));
            walker = inputBuff;
        }
        *walker = c;
        walker++;
    }

    fclose(f);
}

/**
 * Stores memory into persistent file
 * @param tariffList Pointer to list of tariffs
 * @param customerList Pointer to list of customers
 */
void saveFile(int machine, TariffList_t *tariffList, CustomerList_t *customerList)
{
    Customer_t *customer;
    Tariff_t *tariff, *assignedTariff;
    FILE *f;

    if (machine == LINUX)
        f = fopen("../files/tariff.txt", "w");
    else
        f = fopen("../../../files/tariff.txt", "r");


    if (!f) {
        fprintf(stderr, RED"Unable to open tariff.txt\n"RESET);
        exit(-1);
    }

    TLFirst(tariffList);
    while(tariffList->active) {
        tariff = tariffList->active;
        fprintf(f, "%d;%s;%.2lf\n", tariff->id, tariff->name, tariff->price);
        TLNext(tariffList);
    }

    fclose(f);

    if (machine == LINUX)
        f = fopen("../files/customer.txt", "w");
    else
        f = fopen("../../../files/tariff.txt", "r");

    if (!f) {
        fprintf(stderr, RED"Unable to open customer.txt\n"RESET);
        exit(-1);
    }
    
    CLFirst(customerList);
    while (customerList->active) {
        customer = customerList->active;
        fprintf(f, "%d;%s %s;%s;", customer->id, customer->name, customer->surname, customer->phone);

        if (!customer->assignedTariffs)
            fprintf(f, "-\n");
        else {
            TLFirst(customer->assignedTariffs);
            while (customer->assignedTariffs->active) {
                assignedTariff = customer->assignedTariffs->active;
                fprintf(f, "%d", assignedTariff->id);
                if (assignedTariff->next)
                    fprintf(f, ",");
                
                TLNext(customer->assignedTariffs);
            }
            fprintf(f, "\n");
        }

        CLNext(customerList);
    }

    fclose(f);
}
