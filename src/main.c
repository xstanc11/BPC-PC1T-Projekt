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
    char name[MAX_NAME], surname[MAX_NAME], phone[MAX_PHONE], fullName[2 * MAX_NAME];
    do {
        printCustomerMenu();
        scanf("%d", &choice);
        flush_stdin();
        switch (choice) {
        case 1: { // insert customer
            printf(GREEN "Name: " RESET);
            read_line("", name, sizeof(name));
            printf(BLUE "Surname: " RESET);
            read_line("", surname, sizeof(surname));
            printf(VIOLET "Phone: " RESET);
            read_line("", phone, sizeof(phone));
            strncpy(fullName, name, sizeof(name));
            strcat(fullName, " ");
            strcat(fullName, surname);
            CLInsert(-1, fullName, phone, NULL, custList);
            break;
        }
        case 2: { // edit customer
            printf(ORANGE "ID customer: " RESET);
            scanf("%d", &id);
            printf(GREEN "New name (optional, in such case leave blank): " RESET);
            read_line("", name, sizeof(name));
            printf(BLUE "New surname (optional, in such case leave blank): " RESET);
            read_line("", surname, sizeof(surname));
            // printf(VIOLET "New phone (optional, in such case leave blank): " RESET);
            read_line("huhehuhue", phone, sizeof(phone));
            printf("to som%s", phone);
            CLEdit(id, name, surname, phone, custList);
            break;
        }
        case 3: { // delete customer
            int id;
            printf(RED "ID customers to delete: " RESET);
            scanf("%d", &id);
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
    char name[MAX_NAME];
    double price;
    do {
        printTariffMenu();
        scanf("%d", &choice);
        flush_stdin();
        switch (choice) {
        case 1: { // insert tariff
            printf(GREEN "Name: " RESET);
            read_line("", name, sizeof(name));
            printf(BLUE "Price: " RESET);
            scanf("%lf", &price);
            TLInsert(-1, name, price, tariffList);
            break;
        }
        case 2: { // edit tariff
            printf(ORANGE "ID tariff: " RESET);
            scanf("%d", &id);
            printf(GREEN "New name: (optional) " RESET);
            read_line("", name, sizeof(name));
            printf(BLUE "New price: (if wanted to be kept as original, type -1) " RESET);
            scanf("%lf", &price);
            TLEdit(id, name, price, tariffList);
            break;
        }
        case 3: { // delete tariff
            printf(RED "ID tariff to delete: " RESET);
            scanf("%d", &id);
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
        flush_stdin();
        switch (choice) {
        case 1: { // assign tariff
            printf(GREEN "ID customer: " RESET);
            scanf("%d", &custId);
            printf(BLUE "ID tariff: " RESET);
            scanf("%d", &tariffId);
            assignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 2: { // unassign tariff
            printf(ORANGE "ID customer: " RESET);
            scanf("%d", &custId);
            printf(VIOLET "ID tariff: " RESET);
            scanf("%d", &tariffId);
            unassignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 3: { // print customer's tariffs
            printf(YELLOW "ID customer: " RESET);
            scanf("%d", &custId);
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
        flush_stdin();
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