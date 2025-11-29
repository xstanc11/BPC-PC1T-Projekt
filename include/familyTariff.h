// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#pragma once

#include "util.h"

typedef struct FamilyPlan {
	int id;
	char name[MAX_NAME];
	CustomerList_t *assignedCustomers;
	int maxCustomers;
	int customerCount;
	double price;
	struct FamilyPlan* next;
} FamilyPlan_t;

typedef struct {
	FamilyPlan_t *first;
	FamilyPlan_t *active;
} FamilyPlanList_t;

FamilyPlan_t* createFamilyPlan(const char*, int, double);

FamilyPlanList_t* FPLInit();
void FPLDispose(FamilyPlanList_t *);

void FPLFirst(FamilyPlanList_t *);
void FPLNext(FamilyPlanList_t *);

FamilyPlan_t* FPLFindPlanByID(int, FamilyPlan_t *);

void FPLInsert(int, char *, int, double, CustomerList_t *, FamilyPlanList_t *);
void FPLEdit(int, char *, int, double, FamilyPlanList_t *);
void FPLDelete(int, FamilyPlanList_t *);

void addCustomerToFamily(int, int);
void removeCustomerFromFamily(int, int);
void listFamilyPlans();
double calculateFamilyPrice(int, TariffList_t*);
void showFamilyDetails(int, CustomerList_t*, TariffList_t*);
void deleteFamilyPlan(int);
void freeFamilyPlans();
