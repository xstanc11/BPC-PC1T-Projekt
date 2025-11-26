// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileOP.h"

void readFile(TariffList_t *tariffList, CustomerList_t *customerList)
{
    FILE *f;
    char inputBuff[MAX_NAME * 4]; // for good measure
    char *id, name[MAX_NAME], phone[MAX_PHONE], price[MAX_NAME], tariffs[MAX_NAME];
    char c;
    char *walker = inputBuff;
    f = fopen("../files/tariff.txt", "r");
    // handle error while opening file

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

    while ((c = (char)fgetc(f)) != EOF) {
        if (c == '\n') {
            id = strtok(inputBuff, ";");
            strncpy(name, strtok(NULL, ";"), sizeof(name));
            strncpy(phone, strtok(NULL, ";"), sizeof(phone));
            strncpy(tariffs, strtok(NULL, ";"), sizeof(tariffs));

            CLInsert(atoi(id), name, phone, NULL, customerList);
            printf("%s ", tariffs);

            memset(inputBuff, '\0', sizeof(inputBuff));
            walker = inputBuff;
        }
        *walker = c;
        walker++;
    }


    fclose(f);
}