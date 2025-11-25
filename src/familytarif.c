#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

// Forward declarations for structures from main.c
typedef struct Customer Customer;
typedef struct Tariff Tariff;
typedef struct CustomerList CustomerList_t;
typedef struct TariffList TariffList_t;

// Utility functions from main.c
void flush_stdin(void);
void read_line(const char* prompt, char* buf, size_t sz);

// Family Plan structure
typedef struct FamilyPlan {
    int id;
    char name[MAX_NAME];
    int tariff_id;
    int* customer_ids;
    int customer_count;
    int max_customers;
    double discount_percentage;
    struct FamilyPlan* next;
} FamilyPlan;

// Global family plan list
FamilyPlan* family_head = NULL;
int next_family_id = 1;

void printFamilyMenu() {
    printf("==== Family Plan Management ====\n");
    printf("1) Create family plan\n");
    printf("2) Add customer to family\n");
    printf("3) Remove customer from family\n");
    printf("4) List all family plans\n");
    printf("5) Show family plan details\n");
    printf("6) Delete family plan\n");
    printf("0) Back to main menu\n");
    printf("Choice: ");
}

FamilyPlan* createFamilyPlan(const char* name, int tariff_id, double discount) {
    FamilyPlan* plan = malloc(sizeof(FamilyPlan));
    if (!plan) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    plan->id = next_family_id++;
    strncpy(plan->name, name, MAX_NAME - 1);
    plan->name[MAX_NAME - 1] = '\0';
    plan->tariff_id = tariff_id;
    plan->customer_ids = malloc(sizeof(int) * 10);
    plan->customer_count = 0;
    plan->max_customers = 10;
    plan->discount_percentage = discount;
    plan->next = NULL;

    if (!family_head) {
        family_head = plan;
    }
    else {
        FamilyPlan* current = family_head;
        while (current->next) {
            current = current->next;
        }
        current->next = plan;
    }

    printf("Family plan created successfully! ID: %d\n", plan->id);
    return plan;
}

void addCustomerToFamily(int family_id, int customer_id) {
    FamilyPlan* plan = family_head;
    while (plan && plan->id != family_id) {
        plan = plan->next;
    }

    if (!plan) {
        printf("Family plan not found!\n");
        return;
    }

    for (int i = 0; i < plan->customer_count; i++) {
        if (plan->customer_ids[i] == customer_id) {
            printf("Customer already in this family plan!\n");
            return;
        }
    }

    if (plan->customer_count >= plan->max_customers) {
        plan->max_customers *= 2;
        plan->customer_ids = realloc(plan->customer_ids, sizeof(int) * plan->max_customers);
        if (!plan->customer_ids) {
            printf("Memory reallocation failed!\n");
            return;
        }
    }

    plan->customer_ids[plan->customer_count] = customer_id;
    plan->customer_count++;

    printf("Customer added to family plan successfully!\n");
}

void removeCustomerFromFamily(int family_id, int customer_id) {
    FamilyPlan* plan = family_head;
    while (plan && plan->id != family_id) {
        plan = plan->next;
    }

    if (!plan) {
        printf("Family plan not found!\n");
        return;
    }

    for (int i = 0; i < plan->customer_count; i++) {
        if (plan->customer_ids[i] == customer_id) {
            for (int j = i; j < plan->customer_count - 1; j++) {
                plan->customer_ids[j] = plan->customer_ids[j + 1];
            }
            plan->customer_count--;
            printf("Customer removed from family plan!\n");
            return;
        }
    }

    printf("Customer not found in this family plan!\n");
}

void listFamilyPlans() {
    if (!family_head) {
        printf("No family plans available.\n");
        return;
    }

    printf("=== Family Plans ===\n");
    FamilyPlan* current = family_head;
    while (current) {
        printf("ID: %d | Name: %s | Tariff ID: %d | Members: %d | Discount: %.1f%%\n",
            current->id, current->name, current->tariff_id,
            current->customer_count, current->discount_percentage);
        current = current->next;
    }
}

double calculateFamilyPrice(int family_id, TariffList_t* tariffList) {
    FamilyPlan* plan = family_head;
    while (plan && plan->id != family_id) {
        plan = plan->next;
    }

    if (!plan || plan->customer_count == 0) {
        return 0.0;
    }

    double base_price = 20.0;
    double total_price = base_price * plan->customer_count;
    double discount_amount = total_price * (plan->discount_percentage / 100.0);

    return total_price - discount_amount;
}

void showFamilyDetails(int family_id, CustomerList_t* custList, TariffList_t* tariffList) {
    FamilyPlan* plan = family_head;
    while (plan && plan->id != family_id) {
        plan = plan->next;
    }

    if (!plan) {
        printf("Family plan not found!\n");
        return;
    }

    printf("=== Family Plan Details ===\n");
    printf("ID: %d\n", plan->id);
    printf("Name: %s\n", plan->name);
    printf("Tariff ID: %d\n", plan->tariff_id);
    printf("Discount: %.1f%%\n", plan->discount_percentage);
    printf("Total Price: %.2f\n", calculateFamilyPrice(family_id, tariffList));

    printf("Members (%d):\n", plan->customer_count);
    for (int i = 0; i < plan->customer_count; i++) {
        printf("  - Customer ID: %d\n", plan->customer_ids[i]);
    }
}

void deleteFamilyPlan(int family_id) {
    FamilyPlan* current = family_head;
    FamilyPlan* prev = NULL;

    while (current && current->id != family_id) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        printf("Family plan not found!\n");
        return;
    }

    if (prev) {
        prev->next = current->next;
    }
    else {
        family_head = current->next;
    }

    free(current->customer_ids);
    free(current);

    printf("Family plan deleted successfully!\n");
}

void handleFamilyMenu(CustomerList_t* custList, TariffList_t* tariffList) {
    int choice;
    char name[MAX_NAME];
    int family_id, customer_id, tariff_id;
    double discount;

    do {
        printFamilyMenu();
        scanf("%d", &choice);
        flush_stdin();

        switch (choice) {
        case 1: {
            printf("Family plan name: ");
            read_line("", name, sizeof(name));
            printf("Tariff ID: ");
            scanf("%d", &tariff_id);
            printf("Discount percentage (0-50): ");
            scanf("%lf", &discount);

            if (discount < 0 || discount > 50) {
                printf("Invalid discount! Using 10%%\n");
                discount = 10.0;
            }

            createFamilyPlan(name, tariff_id, discount);
            break;
        }
        case 2: {
            printf("Family plan ID: ");
            scanf("%d", &family_id);
            printf("Customer ID to add: ");
            scanf("%d", &customer_id);
            addCustomerToFamily(family_id, customer_id);
            break;
        }
        case 3: {
            printf("Family plan ID: ");
            scanf("%d", &family_id);
            printf("Customer ID to remove: ");
            scanf("%d", &customer_id);
            removeCustomerFromFamily(family_id, customer_id);
            break;
        }
        case 4: {
            listFamilyPlans();
            break;
        }
        case 5: {
            printf("Family plan ID: ");
            scanf("%d", &family_id);
            showFamilyDetails(family_id, custList, tariffList);
            break;
        }
        case 6: {
            printf("Family plan ID to delete: ");
            scanf("%d", &family_id);
            deleteFamilyPlan(family_id);
            break;
        }
        case 0: {
            printf("Returning to main menu...\n");
            break;
        }
        default: {
            printf("Invalid choice!\n");
        }
        }
    } while (choice != 0);
}

void freeFamilyPlans() {
    FamilyPlan* current = family_head;
    while (current) {
        FamilyPlan* temp = current;
        current = current->next;
        free(temp->customer_ids);
        free(temp);
    }
    family_head = NULL;
}