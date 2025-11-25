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
	printf("==== = HLAVNÍ MENU ==== =\n");
	printf("1) Přidat zákazníka\n");
	printf("2) Upravit zákazníka\n");
	printf("3) Smazat zákazníka\n");
	printf("4) Vypsat zákazníky \n");
	printf("5) Přidat tarif\n");
	printf("6) Upravit tarif\n");
	printf("7) Smazat tarif\n");
	printf("8) Vypsat tarify\n");
	printf("9) Přiřadit tarif zákazníkovi\n");
	printf("10) Odebrat tarif zákazníkovi\n");
	printf("11) Vypsat tarify zákazníka\n");
	printf("0) Konec\n");
	printf("Volba: ");
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
                read_line("Jméno: ", name, sizeof(name));
                read_line("Příjmení: ", surname, sizeof(surname));
                read_line("Telefon: ", phone, sizeof(phone));

                strncpy(fullName, name, sizeof(name));
                strcat(fullName, " ");
                strcat(fullName, surname);

                CLInsert(-1, fullName, phone, NULL, custList);
                break;
            }
            case 2: { // edit customer
                printf("ID zákazníka: ");
                scanf("%d", &id);

                read_line("Nové jméno (optional, in such case leave blank): ", name, sizeof(name));
                read_line("Nové příjmení (optional, in such case leave blank): ", surname, sizeof(surname));
                read_line("Nové telefon (optional, in such case leave blank): ", phone, sizeof(phone));

                CLEdit(id, name, surname, phone, custList);
                break;
            }
            case 3: { // delete customer
                int id;
                printf("ID zákazníka k odstranění: ");
                scanf("%d", &id);

                CLDelete(id, custList);
                break;
            }
            case 4: {
                CLPrint(custList);
                break;
            }
            case 5: { // insert tariff
                read_line("Název: ", name, sizeof(name));
                printf("Cena: ");
                scanf("%lf", &price);

                TLInsert(-1, name, price, tariffList);
                break;
            }
            case 6: { // edit tariff
                printf("ID tarifu: ");
                scanf("%d", &id);

                read_line("Nový název: (optional) ", name, sizeof(name));
                printf("Nová cena: (if wanted to be kept as original, type -1) ");
                scanf("%lf", &price);

                TLEdit(id, name, price, tariffList);
                break;
            }
            case 7: { // delete tariff
                printf("ID tarifu k odstranění: ");
                scanf("%d", &id);
                
                TLDelete(id, tariffList);
                break;
            }
            case 8: {
                TLPrint(tariffList);
                break;
            }
            case 9: { // assign tariff
                printf("ID zákazníka: ");
                scanf("%d", &custId);

                printf("ID tarifu: ");
                scanf("%d", &tariffId);

                assignTariff(tariffId, custId, tariffList, custList);
                break;
            }
            case 10: { // unassign tariff
                printf("ID zákazníka: ");
                scanf("%d", &custId);

                printf("ID tarifu: ");
                scanf("%d", &tariffId);

                unassignTariff(tariffId, custId, tariffList, custList);
                break;
            }
            case 11: { // print customer's tariffs
                printf("ID zákazníka: ");
                scanf("%d", &custId);

                printAssignedTariffs(custId, custList);
                break;
            }
            case 0: {
                printf("Ukončuji program...");
                break;
            }
            default: {
                printf("Neplatná volba!");
            }
		}
	} while (choice != 0);

		CLDispose(custList);
		TLDispose(tariffList);
        free(custList);
        free(tariffList);
		return 0;
	}