// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customer.h"
#include "tariff.h"
#include "util.h"
#include "familyTariff.h"
#include "fileOP.h"

void printMainMenu()
{
    printf(RED "==== Main MENU ====" RESET "\n");
    printf(ORANGE "1) Customer Menu" RESET "\n");
    printf(YELLOW "2) Tariff Menu" RESET "\n");
    printf(GREEN "3) Assignment Menu" RESET "\n");
    printf(BLUE "4) Family Plan Menu" RESET "\n");
    printf(INDIGO "0) End" RESET "\n");
    printf(VIOLET "Choice: " RESET);
}

void printCustomerMenu()
{
    printf(RED "==== Customer Menu ====" RESET "\n");
    printf(ORANGE "1) Add customer" RESET "\n");
    printf(YELLOW "2) Edit customer" RESET "\n");
    printf(GREEN "3) Delete customer" RESET "\n");
    printf(BLUE "4) List customers" RESET "\n");
    printf(INDIGO "0) Back to main menu" RESET "\n");
    printf(VIOLET "Choice: " RESET);
}

void printTariffMenu()
{
    printf(RED "==== Tariff Menu ====" RESET "\n");
    printf(ORANGE "1) Add tariff" RESET "\n");
    printf(YELLOW "2) Edit tariff" RESET "\n");
    printf(GREEN "3) Delete tariff" RESET "\n");
    printf(BLUE "4) List tariffs" RESET "\n");
    printf(INDIGO "0) Back to main menu" RESET "\n");
    printf(VIOLET "Choice: " RESET);
}

void printAssignmentMenu()
{
    printf(RED "==== Assignment Menu ====" RESET "\n");
    printf(ORANGE "1) Assign tariff to customer" RESET "\n");
    printf(YELLOW "2) Unassign tariff from customer" RESET "\n");
    printf(GREEN "3) List tariffs assigned to customer" RESET "\n");
    printf(BLUE "0) Back to main menu" RESET "\n");
    printf(INDIGO "Choice: " RESET);
}

void handleCustomerMenu(CustomerList_t *custList)
{
    int choice, id;
    char name[MAX_NAME] = {'\0'}, surname[MAX_NAME] = {'\0'}, phone[MAX_PHONE] = {'\0'}, fullName[2 * MAX_NAME] = {'\0'};
    do {
        printCustomerMenu();
        scanf("%d", &choice);
        flushStdin();
        switch (choice) {
        case 1: { // insert customer
            printf(GREEN "Name: " RESET);
            readLine(name);
            printf(BLUE "Surname: " RESET);
            readLine(surname);
            printf(VIOLET "Phone: " RESET);
            readLine(phone);
            strncpy(fullName, name, sizeof(name));
            strcat(fullName, " ");
            strcat(fullName, surname);
            CLInsert(-1, fullName, phone, NULL, custList);
            break;
        }
        case 2: { // edit customer
            printf(ORANGE "ID customer: " RESET);
            scanf("%d", &id);
            flushStdin();
            printf(GREEN "New name (optional, in such case leave blank): " RESET);
            readLine(name);
            printf(BLUE "New surname (optional, in such case leave blank): " RESET);
            readLine(surname);
            printf(VIOLET "New phone (optional, in such case leave blank): " RESET);
            readLine(phone);
            CLEdit(id, name, surname, phone, custList);
            break;
        }
        case 3: { // delete customer
            int id;
            printf(RED "ID customers to delete: " RESET);
            scanf("%d", &id);
            flushStdin();
            CLDelete(id, custList);
            break;
        }
        case 4: {
            CLPrint(custList);
            break;
        }
        case 0: {
            printf(YELLOW "Returning to main menu..." RESET "\n");
            break;
        }
        default: {
            printf(RED "Invalid choice!" RESET "\n");
        }
        }
    } while (choice != 0);
}

void handleTariffMenu(TariffList_t *tariffList)
{
    int choice, id;
    char name[MAX_NAME] = {'\0'};
    double price;
    do {
        printTariffMenu();
        scanf("%d", &choice);
        flushStdin();
        switch (choice) {
        case 1: { // insert tariff
            printf(GREEN "Name: " RESET);
            readLine(name);
            printf(BLUE "Price: " RESET);
            scanf("%lf", &price);
            flushStdin();
            TLInsert(-1, name, price, tariffList);
            break;
        }
        case 2: { // edit tariff
            printf(ORANGE "ID tariff: " RESET);
            scanf("%d", &id);
            flushStdin();
            printf(GREEN "New name: (optional) " RESET);
            readLine(name);
            printf(BLUE "New price: (if wanted to be kept as original, type -1) " RESET);
            scanf("%lf", &price);
            flushStdin();
            TLEdit(id, name, price, tariffList);
            break;
        }
        case 3: { // delete tariff
            printf(RED "ID tariff to delete: " RESET);
            scanf("%d", &id);
            flushStdin();
            TLDelete(id, tariffList);
            break;
        }
        case 4: {
            TLPrint(tariffList);
            break;
        }
        case 0: {
            printf(YELLOW "Returning to main menu..." RESET "\n");
            break;
        }
        default: {
            printf(RED "Invalid choice!" RESET "\n");
        }
        }
    } while (choice != 0);
}

void handleAssignmentMenu(CustomerList_t *custList, TariffList_t *tariffList)
{
    int choice, custId, tariffId;
    do {
        printAssignmentMenu();
        scanf("%d", &choice);
        flushStdin();
        switch (choice) {
        case 1: { // assign tariff
            printf(GREEN "ID customer: " RESET);
            scanf("%d", &custId);
            flushStdin();
            printf(BLUE "ID tariff: " RESET);
            scanf("%d", &tariffId);
            flushStdin();
            assignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 2: { // unassign tariff
            printf(ORANGE "ID customer: " RESET);
            scanf("%d", &custId);
            flushStdin();
            printf(VIOLET "ID tariff: " RESET);
            scanf("%d", &tariffId);
            flushStdin();
            unassignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 3: { // print customer's tariffs
            printf(YELLOW "ID customer: " RESET);
            scanf("%d", &custId);
            flushStdin();
            printAssignedTariffs(custId, custList);
            break;
        }
        case 0: {
            printf(YELLOW "Returning to main menu..." RESET "\n");
            break;
        }
        default: {
            printf(RED "Invalid choice!" RESET "\n");
        }
        }
    } while (choice != 0);
}

int main()
{
    TariffList_t* tariffList = TLInit();
    CustomerList_t* custList = CLInit();
    readFile(tariffList, custList);
    int choice;
    do {
        printMainMenu();
        scanf("%d", &choice);
        flushStdin();
        switch (choice) {
        case 1: {
            handleCustomerMenu(custList);
            break;
        }
        case 2: {
            handleTariffMenu(tariffList);
            break;
        }
        case 3: {
            handleAssignmentMenu(custList, tariffList);
            break;
        }
        case 4: {
            handleFamilyMenu(custList, tariffList);
            break;
        }
        case 0: {
            printf(VIOLET "Ending program..." RESET "\n");
            break;
        }
        default: {
            printf(RED "Invalid choice!" RESET "\n");
        }
        }
    } while (choice != 0);

    saveFile(tariffList, custList);

    CLDispose(custList);
    TLDispose(tariffList);
    freeFamilyPlans(); // Clean up family plans
    free(custList);
    free(tariffList);


    return 0;
}