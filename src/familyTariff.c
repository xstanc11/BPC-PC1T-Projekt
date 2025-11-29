// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "customer.h"
#include "tariff.h"
#include "familyTariff.h"

int NextFamilyPlanID = 0;

void incrementFamilyPlanId()
{
    NextFamilyPlanID++;
}

/**
 * Initialize Family plan linked list
 * @param list Pointer to family plan list
 * @return Allocated pointer to head of family plan list
 */
FamilyPlanList_t* FPLInit()
{
    FamilyPlanList_t *list = malloc(sizeof(FamilyPlanList_t));

    if (!list) {
        fprintf(stderr, RED"Memory allocation for family tariff list failed\n"RESET);
        exit(-1);
    }

    list->active = NULL;
    list->first = NULL;

    return list;
}

/**
 * Free all allocated nodes
 * @param list Pointer to family plan list
 */
void FPLDispose(FamilyPlanList_t *list)
{
    FPLFirst(list);
    while (list->active) {
        FamilyPlan_t *curr = list->active;
        FPLNext(list);
        if (curr->assignedCustomers != NULL) {
            CLDispose(curr->assignedCustomers);
            free(curr->assignedCustomers);
        }
        free(curr);
    }
}

/**
 * Get first family tariff
 * @param list Pointer to family plan list
 */
void FPLFirst(FamilyPlanList_t *list)
{
    list->active = list->first;
}

/**
 * Get next family plan in list
 * @param list Pointer to family plan list
 */
void FPLNext(FamilyPlanList_t *list)
{
    list->active = list->active->next;
}

/**
 * Returns family plan if ID matches
 * @param name Query ID
 * @param customer Pointer to family plan
 * @return Family plan if found, otherwise NULL
 */
FamilyPlan_t* FPLFindPlanByID(int id, FamilyPlan_t *familyPlan)
{
    if (!familyPlan)
        return NULL;
    if (familyPlan->id == id)
        return familyPlan;
    else
        FPLFindPlanByID(id, familyPlan->next);
}

/**
 * Insert a family plan into the family plan list
 * Insert new family plans with id == -1 to ensure correct functioning of FPLEdit()
 * @param name Family plan name
 * @param maxCustomers Maximum number of assignable customers
 * @param price Family plan price
 * @param customers Pointer to a list of customers
 * @param list Pointer to family plan list
 */
void FPLInsert(int id, char *name, int maxCustomers, double price, CustomerList_t *customers, FamilyPlanList_t *list)
{
	FamilyPlan_t *prev, *new;
	prev = list->first;

    if (FPLFindPlanByID(id, list->first)) {
        printf(RED"Family plan with name %s already exists"RESET"\n", name);
        return;
    }

	new = malloc(sizeof(FamilyPlan_t));
	if (!new) {
        fprintf(stderr, ORANGE"Memory allocation for family plan failed"RESET"\n");
        exit(-1);
    }
    memset(new, 0, sizeof(FamilyPlan_t));

	if (id == -1)
		new->id = NextFamilyPlanID++;
	else
		new->id = id;

    printf("newid %d", new->id);
	strncpy(new->name, name, strlen(name));
	new->assignedCustomers = customers;
	new->maxCustomers = maxCustomers;
	new->price = price;

	FPLFirst(list);

	if (!list->active) { // empty list
		new->next = NULL;
		list->first = new;
		list->active = new;
	} else if (new->id < list->active->id) { // insert as first
		new->next = list->first;
		list->first = new;
		list->active = new;
	} else {
		while (list->active && new->id > list->active->id) {
			prev = list->active;
			FPLNext(list);
		}

		prev->next = new;
		new->next = list->active;
		list->active = new;
	}
}

/**
 * Edit a family plan by ID
 * @param id Family plan ID
 * @param name New family plan name (optional)
 * @param maxCustomers New maximum number of assignable customers (optional, -1 if wanted to be kept original)
 * @param price New tariff price (optional, -1 if wanted to be kept original)
 */
void FPLEdit(int id, char *name, int maxCustomers, double price, FamilyPlanList_t *list)
{
	FamilyPlan_t *plan = FPLFindPlanByID(id, list->first);
	char newName[MAX_NAME] = {'\0'};
	int newMax = 0;
	double newPrice = 0.0;
	CustomerList_t *oldList = plan->assignedCustomers;

	if (!plan) {
		printf(YELLOW"Wrong id (ID = %d), family plan not found"RESET"\n", id);
        return;
	}

	(name[0] != '\0') ? strncpy(newName, name, MAX_NAME - 1) : strncpy(newName, plan->name, MAX_NAME - 1);
    newName[MAX_NAME - 1] = '\0';
	(maxCustomers != -1) ? (newMax = maxCustomers) : (newMax = plan->maxCustomers);
	(price != -1) ? (newPrice = price) : (newPrice = plan->price);

	FPLDelete(plan->id, list);
	FPLInsert(id, newName, newMax, newPrice, oldList, list);
}

/**
 * Delete a family plan by ID
 * @param id Family plan ID
 * @param list Pointer to family plan list
 */
void FPLDelete(int id, FamilyPlanList_t *list)
{
    FamilyPlan_t *plan = FPLFindPlanByID(id, list->first);
    if (!plan) {
        printf(GREEN"Wrong id (ID = %d), family plan not found"RESET"\n", id);
        return;
    }
    FPLFirst(list);

    if (!list->active) {
        printf(BLUE"There are no tariffs in the system"RESET"\n");
        return;
    }

    if (list->first == plan) {
        list->first = plan->next;
		if (plan->assignedCustomers != NULL) {
			CLDispose(plan->assignedCustomers);
			free(plan->assignedCustomers);
		}
        free(plan);
        return;
    }

    while (list->active) {
        if (list->active->next == plan)
            break;
        FPLNext(list);
    }

    list->active->next = plan->next;
    free(plan);
}

/**
 * Print family plans
 * @param list Pointer to family plan list
 */
void FPLPrint(FamilyPlanList_t *list)
{
	FamilyPlan_t *curr;
	FPLFirst(list);
	while (list->active) {
		curr = list->active;
		printf(RED "ID: %d\nname: %s\nmaximum number of assignable customers: %d\nnumber of currently assigned customers: %d\nprice: %.2lf\n\n" RESET, curr->id, curr->name, curr->maxCustomers, curr->customerCount, curr->price);
		FPLNext(list);
	}
}

/**
 * Assign customer to family plan
 * @param customerId ID customer to be assigned
 * @param familyPlanId ID of assignee family plan
 * @param customerList List of available customers
 * @param familyPlanList List of registered family plans
 */
void assignCustomer(int customerId, int familyPlanId, CustomerList_t *customerList, FamilyPlanList_t *familyPlanList)
{
    char newName[2 * MAX_NAME] = {'\0'};
	Customer_t *customer = CLFindCustomerByID(customerId, customerList->first);

	if (!customer) {
        printf(ORANGE"Wrong id (ID = %d), customer not found"RESET"\n", customerId);
        return;
	}

	FamilyPlan_t *plan = FPLFindPlanByID(familyPlanId, familyPlanList->first);
	
	if (!plan) {
		printf(YELLOW"Wrong id (ID = %d), family plan not found"RESET"\n", familyPlanId);
		return;
	}

	if (!plan->assignedCustomers) // no assigned customers
		plan->assignedCustomers = CLInit();
	else {
		if (CLFindCustomerByID(customerId, plan->assignedCustomers->first)) {
            printf(GREEN"Family plan already has this customer (ID = %d) assigned"RESET"\n", customerId);
            return;
        }
	}

    if (plan->customerCount + 1 > plan->maxCustomers) {
        printf("Family plan (ID = %d) already has maximum of assignable customers\n", plan->id);
        return;
    }

    strncpy(newName, customer->name, MAX_NAME - 1);
    strcat(newName, " ");
    strcat(newName, customer->surname);

    plan->customerCount++;

    // tariffs can be null because there is no reason to track that in family plan
	CLInsert(customer->id, newName, customer->phone, NULL, plan->assignedCustomers);
}

/**
 * Assign customer to family plan
 * @param customerId ID customer to be assigned
 * @param familyPlanId ID of assignee family plan
 * @param customerList List of available customers
 * @param familyPlanList List of registered family plans
 */
void unAssignCustomer(int customerId, int familyPlanId, CustomerList_t *customerList, FamilyPlanList_t *familyPlanList)
{
	Customer_t *customer = CLFindCustomerByID(customerId, customerList->first);

	if (!customer) {
        printf(ORANGE"Wrong id (ID = %d), customer not found"RESET"\n", customerId);
        return;
	}

	FamilyPlan_t *plan = FPLFindPlanByID(familyPlanId, familyPlanList->first);
	
	if (!plan) {
		printf(YELLOW"Wrong id (ID = %d), family plan not found"RESET"\n", familyPlanId);
		return;
	}

    if (!CLFindCustomerByID(customerId, plan->assignedCustomers->first)) {
        printf(INDIGO"Family plan doesn't have this customer (ID = %d) assigned"RESET"\n", customerId);
        return;
    }

    plan->customerCount--;
    
    CLDelete(customer->id, plan->assignedCustomers);

    if (!plan->assignedCustomers->first) { // if the customer was the last assigned
        free(plan->assignedCustomers);
        plan->assignedCustomers = NULL;
    }
}

/**
 * Print family plan's assigned customer
 * @param id ID of family plan
 * @param list List of registered family plans
 */
void printAssignedCustomers(int id, FamilyPlanList_t *list)
{ 
	FamilyPlan_t *plan = FPLFindPlanByID(id, list->first);

    if (!plan) {
        printf(VIOLET"Wrong id (ID = %d), family plan not found\n"RESET"", id);
        return;
    }

    if (!plan->assignedCustomers) {
        printf(RED"Family plan (ID = %d) has no customers assigned"RESET"\n", plan->id);
        return;
    }

    CLFirst(plan->assignedCustomers);
    printf(ORANGE"Family plan %s has the following customers assigned: (%d out of %d)"RESET"\n", plan->name, plan->customerCount, plan->maxCustomers);

	Customer_t *curr;

    while (plan->assignedCustomers->active) {
		curr = plan->assignedCustomers->active;
        printf(YELLOW"\tID: %d\n\tname: %s\n\tsurname: %s\n\tphone number: %s"RESET"\n\n", curr->id, curr->name, curr->surname, curr->phone);
        CLNext(plan->assignedCustomers);
    }
}