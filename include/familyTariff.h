// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "util.h"

typedef struct FamilyPlan {
	int id;
	char name[MAX_NAME];
	int tariff_id;
	int* customer_ids;
	int customer_count;
	int max_customers;
	double discount_percentage;
	struct FamilyPlan* next;
} FamilyPlan_t;

void printFamilyMenu();

FamilyPlan_t* createFamilyPlan(const char*, int, double);

void addCustomerToFamily(int, int);
void removeCustomerFromFamily(int, int);
void listFamilyPlans();
double calculateFamilyPrice(int, TariffList_t*);
void showFamilyDetails(int, CustomerList_t*, TariffList_t*);
void deleteFamilyPlan(int);
void handleFamilyMenu(CustomerList_t*, TariffList_t*);
void freeFamilyPlans();
