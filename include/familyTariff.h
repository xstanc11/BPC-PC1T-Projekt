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

void incrementFamilyPlanId();

FamilyPlanList_t* FPLInit();
void FPLDispose(FamilyPlanList_t *);

void FPLFirst(FamilyPlanList_t *);
void FPLNext(FamilyPlanList_t *);

FamilyPlan_t* FPLFindPlanByID(int, FamilyPlan_t *);

void FPLInsert(int, char *, int, double, CustomerList_t *, FamilyPlanList_t *);
void FPLEdit(int, char *, int, double, FamilyPlanList_t *);
void FPLDelete(int, FamilyPlanList_t *);

void FPLPrint(FamilyPlanList_t *);

void assignCustomer(int, int, CustomerList_t *, FamilyPlanList_t *);
void unAssignCustomer(int, int, CustomerList_t *, FamilyPlanList_t *);
void printAssignedCustomers(int, FamilyPlanList_t *);