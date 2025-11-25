#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include "tariff.h"
#include "util.h"
#include "main.h"

void printMainMenu() {
    printf("==== Main MENU ====\n");
    printf("1) Customer Menu\n");
    printf("2) Tariff Menu\n");
    printf("3) Assignment Menu\n");
    printf("0) End\n");
    printf("Choice: ");
}

void printCustomerMenu() {
    printf("==== Customer Menu ====\n");
    printf("1) Add customer\n");
    printf("2) Edit customer\n");
    printf("3) Delete customer\n");
    printf("4) List customers\n");
    printf("0) Back to main menu\n");
    printf("Choice: ");
}

void printTariffMenu() {
    printf("==== Tariff Menu ====\n");
    printf("1) Add tariff\n");
    printf("2) Edit tariff\n");
    printf("3) Delete tariff\n");
    printf("4) List tariffs\n");
    printf("0) Back to main menu\n");
    printf("Choice: ");
}

void printAssignmentMenu() {
    printf("==== Assignment Menu ====\n");
    printf("1) Assign tariff to customer\n");
    printf("2) Unassign tariff from customer\n");
    printf("3) List tariffs assigned to customer\n");
    printf("0) Back to main menu\n");
    printf("Choice: ");
}

void handleCustomerMenu(CustomerList_t* custList) {
    int choice, id;
    char name[MAX_NAME], surname[MAX_NAME], phone[MAX_PHONE], fullName[2 * MAX_NAME];

    do {
        printCustomerMenu();
        scanf("%d", &choice);
        flush_stdin();

        switch (choice) {
        case 1: { // insert customer
            read_line("Name: ", name, sizeof(name));
            read_line("Surname: ", surname, sizeof(surname));
            read_line("Phone: ", phone, sizeof(phone));
            strncpy(fullName, name, sizeof(name));
            strcat(fullName, " ");
            strcat(fullName, surname);
            CLInsert(-1, fullName, phone, NULL, custList);
            break;
        }
        case 2: { // edit customer
            printf("ID customer: ");
            scanf("%d", &id);
            read_line("New name (optional, in such case leave blank): ", name, sizeof(name));
            read_line("New surname (optional, in such case leave blank): ", surname, sizeof(surname));
            read_line("New phone (optional, in such case leave blank): ", phone, sizeof(phone));
            CLEdit(id, name, surname, phone, custList);
            break;
        }
        case 3: { // delete customer
            int id;
            printf("ID customers to delete: ");
            scanf("%d", &id);
            CLDelete(id, custList);
            break;
        }
        case 4: {
            CLPrint(custList);
            break;
        }
        case 0: {
            printf("Returning to main menu...\n");
            break;
        }
        default: {
            printf("Invalid choice!");
        }
        }
    } while (choice != 0);
}

void handleTariffMenu(TariffList_t* tariffList) {
    int choice, id;
    char name[MAX_NAME];
    double price;

    do {
        printTariffMenu();
        scanf("%d", &choice);
        flush_stdin();

        switch (choice) {
        case 1: { // insert tariff
            read_line("Name: ", name, sizeof(name));
            printf("Price: ");
            scanf("%lf", &price);
            TLInsert(-1, name, price, tariffList);
            break;
        }
        case 2: { // edit tariff
            printf("ID tariff: ");
            scanf("%d", &id);
            read_line("New name: (optional) ", name, sizeof(name));
            printf("New price: (if wanted to be kept as original, type -1) ");
            scanf("%lf", &price);
            TLEdit(id, name, price, tariffList);
            break;
        }
        case 3: { // delete tariff
            printf("ID tariff to delete: ");
            scanf("%d", &id);
            TLDelete(id, tariffList);
            break;
        }
        case 4: {
            TLPrint(tariffList);
            break;
        }
        case 0: {
            printf("Returning to main menu...\n");
            break;
        }
        default: {
            printf("Invalid choice!");
        }
        }
    } while (choice != 0);
}

void handleAssignmentMenu(CustomerList_t* custList, TariffList_t* tariffList) {
    int choice, custId, tariffId;

    do {
        printAssignmentMenu();
        scanf("%d", &choice);
        flush_stdin();

        switch (choice) {
        case 1: { // assign tariff
            printf("ID customer: ");
            scanf("%d", &custId);
            printf("ID tariff: ");
            scanf("%d", &tariffId);
            assignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 2: { // unassign tariff
            printf("ID customer: ");
            scanf("%d", &custId);
            printf("ID tariff: ");
            scanf("%d", &tariffId);
            unassignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 3: { // print customer's tariffs
            printf("ID customer: ");
            scanf("%d", &custId);
            printAssignedTariffs(custId, custList);
            break;
        }
        case 0: {
            printf("Returning to main menu...\n");
            break;
        }
        default: {
            printf("Invalid choice!");
        }
        }
    } while (choice != 0);
}

int main() {
    CustomerList_t* custList = CLInit();
    TariffList_t* tariffList = TLInit();
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
        case 0: {
            printf("Ending program...");
            break;
        }
        default: {
            printf("Invalid choice!");
        }
        }
    } while (choice != 0);

    CLDispose(custList);
    TLDispose(tariffList);
    free(custList);
    free(tariffList);
    return 0;
}