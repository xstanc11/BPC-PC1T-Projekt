#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "tariff.h"
#include "util.h"
#include "main.h"
// ==== FULL MENU IMPLEMENTATION ADDED ====
void printMenu() {
	printf(" ==== = HLAVNÍ MENU ==== =");
	printf("1) Přidat zákazníka");
	printf("2) Upravit zákazníka");
	printf("3) Smazat zákazníka");
	printf("4) Vypsat zákazníky ");
	printf("5) Přidat tarif");
	printf("6) Upravit tarif");
	printf("7) Smazat tarif");
	printf("8) Vypsat tarify");
	printf("9) Přiřadit tarif zákazníkovi");
	printf("10) Odebrat tarif zákazníkovi");
	printf("11) Vypsat tarify zákazníka");
	printf("0) Konec");
	printf("Volba: ");
}

int main() {
	CustomerList_t* custList = CLInit();
	TariffList_t* tariffList = TLInit();
	int choice;

	do {
		printMenu();
		scanf("%d", &choice);
		clearInputBuffer();

		switch (choice) {
		case 1: { // insert customer
			int id; char name[MAX_NAME], surname[MAX_NAME], phone[MAX_PHONE];
			printf("ID: "); scanf("%d", &id); clearInputBuffer();
			printf("Jméno: "); fgets(name, MAX_NAME, stdin); stripNewline(name);
			printf("Příjmení: "); fgets(surname, MAX_NAME, stdin); stripNewline(surname);
			printf("Telefon: "); fgets(phone, MAX_PHONE, stdin); stripNewline(phone);
			CLInsert(id, name, surname, NULL, custList);
			break;
		}
		case 2: { // edit customer
			int id; char name[MAX_NAME], surname[MAX_NAME], phone[MAX_PHONE];
			printf("ID zákazníka: "); scanf("%d", &id); clearInputBuffer();
			printf("Nové jméno: "); fgets(name, MAX_NAME, stdin); stripNewline(name);
			printf("Nové příjmení: "); fgets(surname, MAX_NAME, stdin); stripNewline(surname);
			printf("Nový telefon: "); fgets(phone, MAX_PHONE, stdin); stripNewline(phone);
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
			int id; char name[MAX_NAME]; double price;
			printf("ID tarifu: "); scanf("%d", &id); clearInputBuffer();
			printf("Název: "); fgets(name, MAX_NAME, stdin); stripNewline(name);
			printf("Cena: "); scanf("%lf", &price); clearInputBuffer();
			TLInsert(id, name, price, tariffList);
			break;
		}
		case 6: { // edit tariff
			int id; char name[MAX_NAME]; double price;
			printf("ID tarifu: "); scanf("%d", &id); clearInputBuffer();
			printf("Nový název: "); fgets(name, MAX_NAME, stdin); stripNewline(name);
			printf("Nová cena: "); scanf("%lf", &price); clearInputBuffer();
			TLEdit(id, name, price, tariffList);
			break;
		}
		case 7: { // delete tariff
			int id;
			printf("ID tarifu k odstranění: "); scanf("%d", &id);
			TLDelete(id, tariffList);
			break;
		}
		case 8: {
			TLPrint(tariffList);
			break;
		}
		case 9: { // assign tariff
			int custId, tariffId;
			printf("ID zákazníka: "); scanf("%d", &custId);
			printf("ID tarifu: "); scanf("%d", &tariffId);
			assignTariff(custId, tariffId, tariffList, custList);
			break;
		}
		case 10: { // unassign tariff
			int custId, tariffId;
			printf("ID zákazníka: "); scanf("%d", &custId);
			printf("ID tarifu: "); scanf("%d", &tariffId);
			unassignTariff(custId, tariffId, tariffList, custList);
			break;
		}
		case 11: { // print customer's tariffs
			int custId;
			printf("ID zákazníka: "); scanf("%d", &custId);
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
	}
		while (choice != 0);


		CLDispose(custList);
		LDispose(tariffList);
		return 0;
	}