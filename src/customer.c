// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include "customer.h"
#include "util.h"

int NextID = 0;

/**
 * Initialize Customer linked list
 * @param list Pointer to customer list
 */
void CLInit(CustomerList_t *list)
{
    list->active = NULL;
    list->first = NULL;
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

//TODO find/list multiple customers

/**
 * Returns customer if ID matches
 * @param name Query ID
 * @param customer Pointer to customer
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
 * Splits a string consiting of a name and surname into two
 * @param fullName Full name separated by a space
 * @param name Pointer to variable for the name
 * @param name Pointer to variable for the surname
 */
void splitName(char *fullName, char *name, char *surname)
{
    char tmp[2 * MAX_NAME] = {'\0'};
    strncpy(tmp, fullName, strlen(fullName));
    strncpy(name, strtok(tmp, " "), MAX_NAME - 1);
    strncpy(surname, strtok(NULL, " "), MAX_NAME - 1);
}

/**
 * Insert a customer into the customer list
 * @param name Customer name
 * @param phone Phone number
 * @param list Pointer to customer list
 */
void CLInsert(int id, char *fullName, char *phone, CustomerList_t *list)
{
    Customer_t *prev, *new;
    prev = list->first;
    char name[MAX_NAME], surname[MAX_NAME];
    splitName(fullName, name, surname);

    if (CLFindCustomerByName(name, surname, list->first)) {
        printf("Customer with name %s %s already exists\n", name, surname);
        return;
    } else if (CLFindCustomerByPhone(phone, list->first)) {
        printf("Customer with number %s already exists\n", phone);
        return;
    }

    new = malloc(sizeof(Customer_t));
    if (!new) {
        fprintf(stderr, "Memory allocation for customer failed\n");
        exit(-1);
    }
    memset(new, 0, sizeof(Customer_t));

    if (id < 0)
        new->id = NextID++;
    else
        new->id = id;
    strncpy(new->name, name, strlen(name));
    strncpy(new->surname, surname, strlen(surname));
    strncpy(new->phone, phone, strlen(phone));

    if (!list->active) {
        new->next = NULL;
        list->first = new;
        list->active = new;
    } else if (strcmp(surname, list->active->surname) < 0 || (strcmp(surname, list->active->surname) == 0 && strcmp(name, list->active->name) < 0)) {
        CLFirst(list);
        new->next = list->first;
        list->first = new;
        list->active = new;
    } else {
        CLFirst(list);
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
    Customer_t *customer = CLFindCustomerByID(id, list->first);
    char newName[MAX_NAME] = {'\0'}, newSurname[MAX_NAME] = {'\0'}, newPhone[MAX_PHONE] = {'\0'};

    if (!customer) {
        printf("Wrong id (ID = %d), customer not found\n", id);
        return;
    }

    (name) ? strncpy(newName, name, strlen(name)) : strncpy(newName, customer->name, strlen(customer->name));
    (surname) ? strncpy(newSurname, surname, strlen(surname)) : strncpy(newSurname, customer->surname, strlen(customer->surname));
    (phone) ? strncpy(newPhone, phone, strlen(phone)) : strncpy(newPhone, customer->phone, strlen(customer->phone));
    
    CLDelete(customer->id, list);
    strcat(newName, " ");
    strcat(newName, newSurname);
    CLInsert(id, newName, newPhone, list);
}

// TODO handle tarif assignment/removal and stuff

/**
 * Delete a customer by ID
 * @param id Customer ID
 * @param list Pointer to customer list
 */
void CLDelete(int id, CustomerList_t *list)
{
    Customer_t *customer = CLFindCustomerByID(id, list->first);
    if (!customer) {
        printf("Wrong id (ID = %d), customer not found\n", id);
        return;
    }
    CLFirst(list);

    if (!list->active) {
        printf("There are no customers in the system\n");
        return;
    } else if (list->first == customer) {
        list->first = customer->next;
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
        printf("ID: %d\nname: %s\nsurname: %s\nphone number: %s\n", list->active->id, list->active->name, list->active->surname, list->active->phone);
        CLNext(list);
    }
}