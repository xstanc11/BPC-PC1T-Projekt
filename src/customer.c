// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customer.h"
#include "util.h"

int NextCustID = 0;

void incrementCustomerId()
{
    NextCustID++;
}

/**
 * Initialize Customer linked list
 * @param list Pointer to customer list
 * @return Allocated pointer to head of customer list
 */
CustomerList_t* CLInit()
{
    CustomerList_t *list = malloc(sizeof(CustomerList_t));

    if (!list) {
        fprintf(stderr, RED"Memory allocation for customer list failed\n"RESET);
        exit(-1);
    }

    list->active = NULL;
    list->first = NULL;

    return list;
}

/**
 * Free all allocated nodes
 * @param list Pointer to customer list
 */
void CLDispose(CustomerList_t *list)
{
    CLFirst(list);
    while (list->active) {
        Customer_t *curr = list->active;
        CLNext(list);
        if (curr->assignedTariffs != NULL) {
            TLDispose(curr->assignedTariffs);
            free(curr->assignedTariffs);
        }
        free(curr);
    }
}

/**
 * Get first customer
 * @param list Pointer to customer list
 */
void CLFirst(CustomerList_t *list)
{
    list->active = list->first;
}

/**
 * Get next customer in list
 * @param list Pointer to customer list
 */
void CLNext(CustomerList_t *list)
{
    list->active = list->active->next;
}

/**
 * Returns customer if name matches
 * @param name Query name
 * @param customer Pointer to customer
 * @return Customer if found, otherwise NULL
 */
Customer_t* CLFindCustomerByName(char *name, char *surname, Customer_t *customer)
{
    if (!customer)
        return NULL;
    if (!strcmp(customer->name, name) && !strcmp(customer->surname, surname))
        return customer;
    else
        CLFindCustomerByName(name, surname, customer->next);
}

/**
 * Returns customer if ID matches
 * @param name Query ID
 * @param customer Pointer to customer
 * @return Customer if found, otherwise NULL
 */
Customer_t* CLFindCustomerByID(int id, Customer_t *customer)
{
    if (!customer)
        return NULL;
    if (customer->id == id)
        return customer;
    else
        CLFindCustomerByID(id, customer->next);
}

/**
 * Returns customer if phone number matches
 * @param name Query number
 * @param customer Pointer to customer
 * @return Customer if found, otherwise NULL
 */
Customer_t* CLFindCustomerByPhone(char *phone, Customer_t *customer)
{
    if (!customer)
        return NULL;
    if (!strcmp(customer->phone, phone))
        return customer;
    else
        CLFindCustomerByPhone(phone, customer->next);
}

/**
 * Insert a customer into the customer list.
 * Insert new customers with id == -1 to ensure correct functioning of CLEdit()
 * @param fullName Customer name
 * @param phone Phone number
 * @param tariffs Pointer to a list of tariffs
 * @param list Pointer to customer list
 */
void CLInsert(int id, char *fullName, char *phone, TariffList_t *tariffs, CustomerList_t *list)
{
    Customer_t *prev, *new;
    prev = list->first;
    char name[MAX_NAME], surname[MAX_NAME];
    splitName(fullName, name, surname);

    if (CLFindCustomerByName(name, surname, list->first)) {
        printf(ORANGE "Customer with name %s %s already exists" RESET "\n", name, surname);
        return;
    } else if (CLFindCustomerByPhone(phone, list->first)) {
        printf(YELLOW"Customer with number %s already exists"RESET"\n", phone);
        return;
    }

    new = malloc(sizeof(Customer_t));
    if (!new) {
        fprintf(stderr, GREEN"Memory allocation for customer failed"RESET"\n");
        exit(-1);
    }
    memset(new, 0, sizeof(Customer_t));

    if (id == -1)
        new->id = NextCustID++;
    else
        new->id = id;

    strncpy(new->name, name, MAX_NAME - 1);
    new->name[MAX_NAME - 1] = '\0';
    strncpy(new->surname, surname, MAX_NAME - 1);
    new->surname[MAX_NAME - 1] = '\0';
    strncpy(new->phone, phone, MAX_PHONE - 1);
    new->phone[MAX_PHONE - 1] = '\0';
    new->assignedTariffs = tariffs;

    CLFirst(list);

    if (!list->active) { // empty list
        new->next = NULL;
        list->first = new;
        list->active = new;
    } else if (strcmp(surname, list->active->surname) < 0 || (strcmp(surname, list->active->surname) == 0 && strcmp(name, list->active->name) < 0)) { // insert as first
        new->next = list->first;
        list->first = new;
        list->active = new;
    } else {
        while (list->active &&  (strcmp(surname, list->active->surname) > 0 || (strcmp(surname, list->active->surname) == 0 && strcmp(name, list->active->name) > 0))) {
            prev = list->active;
            CLNext(list);
        }
        
        prev->next = new;
        new->next = list->active;
        list->active = new;
    }
}

/**
 * Edit a customer by ID
 * @param id Customer ID
 * @param name New customer name (optional)
 * @param surname New customer surname (optional) 
 * @param phone New phone number (optional)
 */
void CLEdit(int id, char *name, char *surname, char *phone, CustomerList_t *list)
{
    // TODO handle if family plan has this customer assigned
    Customer_t *customer = CLFindCustomerByID(id, list->first);
    char newName[2 * MAX_NAME] = {'\0'}, newSurname[MAX_NAME] = {'\0'}, newPhone[MAX_PHONE] = {'\0'};
    TariffList_t *oldList = customer->assignedTariffs; // TODO bug, assign after checking

    if (!customer) {
        printf(BLUE"Wrong id (ID = %d), customer not found"RESET"\n", id);
        return;
    }

    (name[0] != '\0') ? strncpy(newName, name, MAX_NAME - 1) : strncpy(newName, customer->name, MAX_NAME - 1);
    (surname[0] != '\0') ? strncpy(newSurname, surname, MAX_NAME - 1) : strncpy(newSurname, customer->surname, MAX_NAME - 1);
    (phone[0] != '\0') ? strncpy(newPhone, phone, MAX_PHONE - 1) : strncpy(newPhone, customer->phone, MAX_PHONE - 1);
    newName[MAX_NAME - 1] = '\0';
    newSurname[MAX_NAME - 1] = '\0';
    newPhone[MAX_PHONE - 1] = '\0';

    CLDelete(customer->id, list);
    strcat(newName, " ");
    strcat(newName, newSurname);
    CLInsert(id, newName, newPhone, oldList, list);
}

/**
 * Delete a customer by ID
 * @param id Customer ID
 * @param list Pointer to customer list
 */
void CLDelete(int id, CustomerList_t *list)
{
    // TODO handle if family plan has this customer assigned
    Customer_t *customer = CLFindCustomerByID(id, list->first);
    if (!customer) {
        printf(INDIGO"Wrong id (ID = %d), customer not found"RESET"\n", id);
        return;
    }
    CLFirst(list);

    if (!list->active) {
        printf(VIOLET"There are no customers in the system"RESET"\n");
        return;
    }

    if (list->first == customer) {
        list->first = customer->next;
        if (customer->assignedTariffs != NULL) {
            TLDispose(customer->assignedTariffs);
            free(customer->assignedTariffs);
        }
        free(customer);
        return;
    }

    while (list->active) {
        if (list->active->next == customer)
            break;
        CLNext(list);
    }

    list->active->next = customer->next;
    free(customer);
}

/**
 * Print customers
 * @param list Pointer to customer list
 */
void CLPrint(CustomerList_t *list)
{
    CLFirst(list);
    while (list->active) {
        printf(RED"ID: %d\nname: %s\nsurname: %s\nphone number: %s"RESET"\n\n", list->active->id, list->active->name, list->active->surname, list->active->phone);
        CLNext(list);
    }
}

// TODO is assigning a tariff to customer the same as assigning customer to tariff??
/**
 * Assign a tariff to customer
 * @param tariffId ID of tariff to be assigned
 * @param customerId ID of assignee customer
 * @param tariffList List of available tariffs
 * @param customerList List of registered customers
 */
void assignTariff(int tariffId, int customerId, TariffList_t *tariffList, CustomerList_t *customerList)
{
    Tariff_t *tariff = TLFindTariffByID(tariffId, tariffList->first);

    if (!tariff) {
        printf(ORANGE"Wrong id (ID = %d), tariff not found"RESET"\n", tariffId);
        return;
    }

    Customer_t *customer = CLFindCustomerByID(customerId, customerList->first);

    if (!customer) {
        printf(YELLOW"Wrong id (ID = %d), customer not found"RESET"\n", customerId);
        return;
    }

    if (!customer->assignedTariffs) // no assigned tariffs
        customer->assignedTariffs = TLInit(); // no need to check failed malloc, TLInit() already does that
    else {
        if (TLFindTariffByID(tariffId, customer->assignedTariffs->first)) {
            printf(GREEN"Customer already has this tariff (ID = %d) assigned"RESET"\n", tariffId);
            return;
        }
    }

    TLInsert(tariff->id, tariff->name, tariff->price, customer->assignedTariffs);
}

/**
 * Unassign a tariff to customer
 * @param tariffId ID of tariff to be unassigned
 * @param customerId ID of assignee customer
 * @param tariffList List of available tariffs
 * @param customerList List of registered customers
 */
void unAssignTariff(int tariffId, int customerId, TariffList_t *tariffList, CustomerList_t *customerList)
{
    Tariff_t *tariff = TLFindTariffByID(tariffId, tariffList->first);

    if (!tariff) {
        printf(GREEN"Wrong id (ID = %d), tariff not found"RESET"\n", tariffId);
        return;
    }

    Customer_t *customer = CLFindCustomerByID(customerId, customerList->first);

    if (!customer) {
        printf(BLUE"Wrong id (ID = %d), customer not found"RESET"\n", customerId);
        return;
    }

    if (!TLFindTariffByID(tariffId, customer->assignedTariffs->first)) {
        printf(INDIGO"Customer doesn't have this tariff (ID = %d) assigned"RESET"\n", tariffId);
        return;
    }

    TLDelete(tariff->id, customer->assignedTariffs);

    if (!customer->assignedTariffs->first) { // if the tariff was the last assigned
        free(customer->assignedTariffs);
        customer->assignedTariffs = NULL;
    }
}

/**
 * Print customer's assigned tariffs
 * @param id ID of customer
 * @param list List of registered customers
 */
void printAssignedTariffs(int id, CustomerList_t *list)
{ 
    Customer_t *customer = CLFindCustomerByID(id, list->first);

    if (!customer) {
        printf(VIOLET"Wrong id (ID = %d), customer not found\n"RESET"", id);
        return;
    }

    if (!customer->assignedTariffs) {
        printf(RED"Customer (ID = %d) has no tariffs assigned"RESET"\n", customer->id);
        return;
    }

    TLFirst(customer->assignedTariffs);
    printf(ORANGE"Customer %s %s has the following tariffs assigned:"RESET"\n", customer->name, customer->surname);

    while (customer->assignedTariffs->active) {
        printf(YELLOW"\tID: %d\n\tname: %s\n\tprice: %lf"RESET"\n\n", customer->assignedTariffs->active->id, customer->assignedTariffs->active->name, customer->assignedTariffs->active->price);
        TLNext(customer->assignedTariffs);
    }
}