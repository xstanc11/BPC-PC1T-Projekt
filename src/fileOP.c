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
    char *token;
    if (tariffs[0] == '-')
        return;

    token = strtok(tariffs, ",");
    while (token != NULL) {
        assignTariff(atoi(token), id, tariffList, customerList);
        token = strtok(NULL, ",");
    }
}

/**
 * Parses out customers during file read and assigns them correctly
 * @param assignedCustomers Pointer to list of customers to be assigned
 * @param id Id of family plan
 * @param customerList Pointer to list of customers
 * @param familyPlanList Pointer to list of family plan
 */
void parseCustomers(char *assignedCustomers, int id, CustomerList_t *customerList, FamilyPlanList_t *familyPlanList)
{
    char *token, *saveptr;
    if (assignedCustomers[0] == '-')
        return;
    
    token = strtok_r(assignedCustomers, ",", &saveptr);
    while (token != NULL) {
        assignCustomer(atoi(token), id, customerList, familyPlanList);
        token = strtok_r(NULL, ",", &saveptr);
    }
}

/**
 * Reads from persistent file into memory
 * @param tariffList Pointer to list of tariffs
 * @param customerList Pointer to list of customers
 */
void readFile(int machine, TariffList_t *tariffList, CustomerList_t *customerList, FamilyPlanList_t *familyPlanList)
{
    FILE *f;
    char inputBuff[MAX_NAME * 4] = {'\0'}; // for good measure
    char *id, maxUsers[MAX_NAME] = {'\0'}, name[MAX_NAME] = {'\0'}, phone[MAX_PHONE] = {'\0'}, price[MAX_NAME] = {'\0'}, tariffs[MAX_NAME] = {'\0'}, assignedCustomers[MAX_NAME] = {'\0'};
    char c;
    char *walker = inputBuff;

    // load tariffs
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

            incrementTariffId();
        }
        *walker = c;
        walker++;
    }

    fclose(f);

    // load customers
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

            incrementCustomerId();
        }
        *walker = c;
        walker++;
    }

    fclose(f);

    // load family tariffs
    if (machine == LINUX)
        f = fopen("../files/familyTariff.txt", "r");
    else
        f = fopen("../../../files/familyTariff.txt", "r");

    if (!f) {
        fprintf(stderr, RED"Unable to open familyTariff.txt\n"RESET);
        exit(-1);
    }

    while ((c = (char)fgetc(f)) != EOF) {
        if (c == '\n') {
            id = strtok(inputBuff, ";");
            strncpy(name, strtok(NULL, ";"), sizeof(name));
            strncpy(maxUsers, strtok(NULL, ";"), sizeof(maxUsers));
            strncpy(price, strtok(NULL, ";"), sizeof(price));
            strncpy(assignedCustomers, strtok(NULL, ";"), sizeof(assignedCustomers));

            FPLInsert(atoi(id), name, atoi(maxUsers), atol(price), NULL, familyPlanList);
            parseCustomers(assignedCustomers, atoi(id), customerList, familyPlanList);

            memset(inputBuff, '\0', sizeof(inputBuff));
            walker = inputBuff;

            incrementFamilyPlanId();
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
void saveFile(int machine, TariffList_t *tariffList, CustomerList_t *customerList, FamilyPlanList_t *familyPlanList)
{
    Customer_t *customer, *assignedCustomer;
    Tariff_t *tariff, *assignedTariff;
    FamilyPlan_t *familyPlan;
    FILE *f;

    if (machine == LINUX)
        f = fopen("../files/tariff.txt", "w");
    else
        f = fopen("../../../files/tariff.txt", "w");


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
        f = fopen("../../../files/customer.txt", "w");

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

    if (machine == LINUX)
        f = fopen("../files/familyTariff.txt", "w");
    else
        f = fopen("../../../files/familyTariff.txt", "w");

    if (!f) {
        fprintf(stderr, RED"Unable to open familyTariff.txt\n"RESET);
        exit(-1);
    }

    FPLFirst(familyPlanList);
    while (familyPlanList->active) {
        familyPlan = familyPlanList->active;
        fprintf(f, "%d;%s;%d;%.2lf;", familyPlan->id, familyPlan->name, familyPlan->maxCustomers, familyPlan->price);

        if (!familyPlan->assignedCustomers)
            fprintf(f, "-\n");
        else {
            CLFirst(familyPlan->assignedCustomers);
            while (familyPlan->assignedCustomers->active) {
                assignedCustomer = familyPlan->assignedCustomers->active;
                fprintf(f, "%d", assignedCustomer->id);
                if (assignedCustomer->next)
                    fprintf(f, ",");

                CLNext(familyPlan->assignedCustomers);
            }
            fprintf(f, "\n");
        }

        FPLNext(familyPlanList);
    }

    fclose(f);
}
