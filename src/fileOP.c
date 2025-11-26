// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileOP.h"

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

void readFile(TariffList_t *tariffList, CustomerList_t *customerList)
{
    FILE *f;
    char inputBuff[MAX_NAME * 4] = {'\0'}; // for good measure
    char *id, name[MAX_NAME], phone[MAX_PHONE], price[MAX_NAME], tariffs[MAX_NAME];
    char c;
    char *walker = inputBuff;
    f = fopen("../files/tariff.txt", "r");
    
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

    f = fopen("../files/customer.txt", "r");

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

void saveFile(TariffList_t *tarifList, CustomerList_t *customerList)
{

}
