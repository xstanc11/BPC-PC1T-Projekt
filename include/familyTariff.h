// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#define MAX_NAME 100
#define RED     "\033[31m"
#define ORANGE  "\033[33m"
#define YELLOW  "\033[93m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define INDIGO  "\033[35m"
#define VIOLET  "\033[95m"
#define RESET   "\033[0m"

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
