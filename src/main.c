#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include "tariff.h"
#include "util.h"
#include "main.h"
// ==== FULL MENU IMPLEMENTATION ADDED ====

// for inspiration
// void readData()
// {
    // FILE *f;
    // f = fopen("kekw.txt", 'r');
    // read line from txt file
    // car buff[x];
    // "0;kekw;69.99"
    // id, name, price;
    // TLInsert(id, name, price);

// }

void printMenu() {
	printf("==== = Main MENU ==== =\n");
	printf("1) Add customer\n");
	printf("2) Edit customer\n");
	printf("3) Delete customer\n");
	printf("4) List customers \n");
	printf("5) Add tariff\n");
	printf("6) Edit tariff\n");
	printf("7) Delete tariff\n");
	printf("8) List tariff\n");
	printf("9) Assign tariff to customer\n");
	printf("10) Unassign tariff from customer\n");
	printf("11) List tariffs assign to customer\n");
	printf("0) End\n");
	printf("Choice: ");
}

int main() {
	CustomerList_t* custList = CLInit();
	TariffList_t* tariffList = TLInit();
	int choice, id, custId, tariffId;
    char name[MAX_NAME], surname[MAX_NAME], phone[MAX_PHONE], fullName[2 * MAX_NAME];
    double price;

	do {
		printMenu();
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
            case 5: { // insert tariff
                read_line("Name: ", name, sizeof(name));
                printf("Price: ");
                scanf("%lf", &price);

                TLInsert(-1, name, price, tariffList);
                break;
            }
            case 6: { // edit tariff
                printf("ID tariff: ");
                scanf("%d", &id);

                read_line("New name: (optional) ", name, sizeof(name));
                printf("¨New price: (if wanted to be kept as original, type -1) ");
                scanf("%lf", &price);

                TLEdit(id, name, price, tariffList);
                break;
            }
            case 7: { // delete tariff
                printf("ID tariff to delete: ");
                scanf("%d", &id);
                
                TLDelete(id, tariffList);
                break;
            }
            case 8: {
                TLPrint(tariffList);
                break;
            }
            case 9: { // assign tariff
                printf("ID customer: ");
                scanf("%d", &custId);

                printf("ID tariff: ");
                scanf("%d", &tariffId);

                assignTariff(tariffId, custId, tariffList, custList);
                break;
            }
            case 10: { // unassign tariff
                printf("ID customer: ");
                scanf("%d", &custId);

                printf("ID tariff: ");
                scanf("%d", &tariffId);

                unassignTariff(tariffId, custId, tariffList, custList);
                break;
            }
            case 11: { // print customer's tariffs
                printf("ID customer: ");
                scanf("%d", &custId);

                printAssignedTariffs(custId, custList);
                break;
            }
            case 0: {
                printf("Ending program...");
                break;
            }
            default: {
                printf("Invalide choice!");
            }
		}
	} while (choice != 0);

		CLDispose(custList);
		TLDispose(tariffList);
        free(custList);
        free(tariffList);
		return 0;
	}