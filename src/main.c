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

void printFamilyMenu()
{
	printf(RED"==== Family Plan Management ===="RESET"\n");
	printf(ORANGE"1) Create family plan"RESET"\n");
	printf(YELLOW"2) Edit family plan"RESET"\n");
	printf(GREEN"3) Delete family plan"RESET"\n");
	printf(BLUE"4) List all family plans"RESET"\n");
	printf(INDIGO"5) Assign customer to family plan "RESET"\n");
	printf(VIOLET"6) Unassign customer from family plan"RESET"\n");
	printf(RED"7) List customers assigned to family plan"RESET"\n");
	printf(ORANGE"0) Back to main menu"RESET"\n");
	printf(YELLOW"Choice: "RESET);
}

void handleCustomerMenu(CustomerList_t *custList, FamilyPlanList_t *familyPlanList)
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
            readLine(name, sizeof(name));
            printf(BLUE "Surname: " RESET);
            readLine(surname, sizeof(surname));
            printf(VIOLET "Phone: " RESET);
            readLine(phone, sizeof(phone));
            strncpy(fullName, name, sizeof(name));
            strcat(fullName, " ");
            strcat(fullName, surname);
            CLInsert(-1, fullName, phone, NULL, custList);
            break;
        }
        case 2: { // edit customer
            printf(ORANGE "Customer ID: " RESET);
            scanf("%d", &id);
            flushStdin();
            printf(GREEN "New name (optional): " RESET);
            readLine(name, sizeof(name));
            printf(BLUE "New surname (optional): " RESET);
            readLine(surname, sizeof(surname));
            printf(VIOLET "New phone (optional): " RESET);
            readLine(phone, sizeof(phone));
            CLEdit(id, name, surname, phone, custList);

            // update customer list in family plans
            FPLFirst(familyPlanList);
            while (familyPlanList->active) {
                if (familyPlanList->active->assignedCustomers) {
                    if (CLFindCustomerByID(id, familyPlanList->active->assignedCustomers->first))
                        CLEdit(id, name, surname, phone, familyPlanList->active->assignedCustomers);
                }
                FPLNext(familyPlanList);
            }
            break;
        }
        case 3: { // delete customer
            int id;
            printf(RED "Customers ID: " RESET);
            scanf("%d", &id);
            flushStdin();

            // update customer list in family plans
            FPLFirst(familyPlanList);
            while (familyPlanList->active) {
                if (familyPlanList->active->assignedCustomers) {
                    if (CLFindCustomerByID(id, familyPlanList->active->assignedCustomers->first))
                        unAssignCustomer(id, familyPlanList->active->id, custList, familyPlanList);
                }
                FPLNext(familyPlanList);
            }
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

void handleTariffMenu(CustomerList_t *custList, TariffList_t *tariffList)
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
            readLine(name, sizeof(name));
            printf(BLUE "Price: " RESET);
            scanf("%lf", &price);
            flushStdin();
            TLInsert(-1, name, price, tariffList);
            break;
        }
        case 2: { // edit tariff
            printf(ORANGE "Tariff ID: " RESET);
            scanf("%d", &id);
            flushStdin();
            printf(GREEN "New name: (optional) " RESET);
            readLine(name, sizeof(name));
            printf(BLUE "New price (if wanted to be kept as original, type -1): " RESET);
            scanf("%lf", &price);
            flushStdin();

            CLFirst(custList);
            while (custList->active) {
                if (custList->active->assignedTariffs) {
                    if (TLFindTariffByID(id, custList->active->assignedTariffs->first))
                        TLEdit(id, name, price, custList->active->assignedTariffs);
                }
                CLNext(custList);
            }
            TLEdit(id, name, price, tariffList);
            break;
        }
        case 3: { // delete tariff
            printf(RED "Tariff ID: " RESET);
            scanf("%d", &id);
            flushStdin();

            CLFirst(custList);
            while (custList->active) {
                if (custList->active->assignedTariffs) {
                    if (TLFindTariffByID(id, custList->active->assignedTariffs->first))
                        unAssignTariff(id, custList->active->id, tariffList, custList);
                }
                CLNext(custList);
            }
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
            printf(GREEN "Customer ID: " RESET);
            scanf("%d", &custId);
            flushStdin();
            printf(BLUE "Tariff ID: " RESET);
            scanf("%d", &tariffId);
            flushStdin();
            assignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 2: { // unassign tariff
            printf(ORANGE "Customer ID: " RESET);
            scanf("%d", &custId);
            flushStdin();
            printf(VIOLET "Tariff ID: " RESET);
            scanf("%d", &tariffId);
            flushStdin();
            unAssignTariff(tariffId, custId, tariffList, custList);
            break;
        }
        case 3: { // print customer's tariffs
            printf(YELLOW "Customer ID: " RESET);
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

void handleFamilyMenu(CustomerList_t* custList, TariffList_t* tariffList, FamilyPlanList_t *familyPlanList)
{
	int choice, id, familyPlanId, custId, tariffId, max;
	char name[MAX_NAME] = {'\0'};
    double price;

	do {
		printFamilyMenu();
		scanf("%d", &choice);
		flushStdin();

		switch (choice) {
		case 1: { // insert family plan
			printf(BLUE"Family plan name: "RESET);
			readLine(name, sizeof(name));
			printf(INDIGO"Maximum number of assignable customers: "RESET);
			scanf("%d", &max);
			flushStdin();
			printf(RED"Family tariff price: "RESET);
			scanf("%lf", &price);
			flushStdin();
            FPLInsert(-1, name, max, price, NULL, familyPlanList);
			break;
		}
		case 2: { // edit family plan
			printf(YELLOW"Family plan ID: "RESET);
			scanf("%d", &id);
			flushStdin();
			printf(GREEN"New name (optional): "RESET);
            readLine(name, sizeof(name));
			printf(GREEN"New maximum of assignable customers (if wanted to be kept as original, type -1): "RESET);
            scanf("%d", &max);
            flushStdin();
            printf(GREEN"New price (if wanted to be kept as original, type -1): "RESET);
            scanf("%lf", &price);
            flushStdin();
            FPLEdit(id, name, max, price, familyPlanList);
			break;
		}
		case 3: { // delete family plan
			printf(BLUE"Family plan ID: "RESET);
			scanf("%d", &id);
			flushStdin();
            FPLDelete(id, familyPlanList);
			break;
		}
		case 4: { // print family plans
            FPLPrint(familyPlanList);
			break;
		}
		case 5: { // assign customer to family plan
			printf(RED"Customer ID: "RESET);
			scanf("%d", &custId);
			flushStdin();
			printf(RED"Family plan ID: "RESET);
			scanf("%d", &familyPlanId);
			flushStdin();
            assignCustomer(custId, familyPlanId, custList, familyPlanList);
			break;
		}
		case 6: { // unassign customer from family plan
			printf(ORANGE"Customer ID: "RESET);
            scanf("%d", &custId);
            flushStdin();
			printf(ORANGE"Family plan ID: "RESET);
			scanf("%d", &familyPlanId);
			flushStdin();
            unAssignCustomer(custId, familyPlanId, custList, familyPlanList);
			break;
		}
        case 7: { // print family plan's customers
            printf(RED"Family plan ID: "RESET);
            scanf("%d", &familyPlanId);
            flushStdin();
            printAssignedCustomers(familyPlanId, familyPlanList);
            break;
        }
		case 0: {
			printf(YELLOW"Returning to main menu..."RESET"\n");
			break;
		}
		default: {
			printf(GREEN"Invalid choice!"RESET"\n");
		}
		}
	} while (choice != 0);
}

int main()
{
    int machine;
    printf(GREEN "Specify the OS of the machine: (LINUX = 0, WINDOWS = 1): " RESET);
    scanf("%d", &machine);
    flushStdin();
    if (machine != LINUX && machine != WINDOWS)
        exit(-1);

    TariffList_t *tariffList = TLInit();
    CustomerList_t *custList = CLInit();
    FamilyPlanList_t *familyPlanList = FPLInit();

    readFile(machine, tariffList, custList, familyPlanList);
    int choice;
    do {
        printMainMenu();
        scanf("%d", &choice);
        flushStdin();
        switch (choice) {
        case 1: {
            handleCustomerMenu(custList, familyPlanList);
            break;
        }
        case 2: {
            handleTariffMenu(custList, tariffList);
            break;
        }
        case 3: {
            handleAssignmentMenu(custList, tariffList);
            break;
        }
        case 4: {
            handleFamilyMenu(custList, tariffList, familyPlanList);
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

    saveFile(machine, tariffList, custList, familyPlanList);

    CLDispose(custList);
    TLDispose(tariffList);
    FPLDispose(familyPlanList);
    free(custList);
    free(tariffList);
    free(familyPlanList);

    return 0;
}