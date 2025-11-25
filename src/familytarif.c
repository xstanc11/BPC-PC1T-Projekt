#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define RED     "\033[31m"
#define ORANGE  "\033[33m"
#define YELLOW  "\033[93m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define INDIGO  "\033[35m"
#define VIOLET  "\033[95m"
#define RESET   "\033[0m"

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
	printf(RED"==== Family Plan Management ====R"RESET"\n");
	printf(ORANGE"1) Create family plan"RESET"\n");
	printf(YELLOW"2) Add customer to family"RESET"\n");
	printf(GREEN"3) Remove customer from family"RESET"\n");
	printf(BLUE"4) List all family plans"RESET"\n");
	printf(INDIGO"5) Show family plan details"RESET"\n");
	printf(VIOLET"6) Delete family plan"RESET"\n");
	printf(RED"0) Back to main menu"RESET"\n");
	printf(ORANGE"Choice: ");
}

FamilyPlan* createFamilyPlan(const char* name, int tariff_id, double discount) {
	FamilyPlan* plan = malloc(sizeof(FamilyPlan));
	if (!plan) {
		printf(YELLOW"Memory allocation failed!"RESET"\n");
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

	printf(GREEN"Family plan created successfully! ID: %d"RESET"\n", plan->id);
	return plan;
}

void addCustomerToFamily(int family_id, int customer_id) {
	FamilyPlan* plan = family_head;
	while (plan && plan->id != family_id) {
		plan = plan->next;
	}

	if (!plan) {
		printf(BLUE"Family plan not found!"RESET"\n");
		return;
	}

	for (int i = 0; i < plan->customer_count; i++) {
		if (plan->customer_ids[i] == customer_id) {
			printf(INDIGO"Customer already in this family plan!"RESET"\n");
			return;
		}
	}

	if (plan->customer_count >= plan->max_customers) {
		plan->max_customers *= 2;
		plan->customer_ids = realloc(plan->customer_ids, sizeof(int) * plan->max_customers);
		if (!plan->customer_ids) {
			printf(VIOLET"Memory reallocation failed!"RESET"\n");
			return;
		}
	}

	plan->customer_ids[plan->customer_count] = customer_id;
	plan->customer_count++;

	printf(RED"Customer added to family plan successfully!"RESET"\n");
}

void removeCustomerFromFamily(int family_id, int customer_id) {
	FamilyPlan* plan = family_head;
	while (plan && plan->id != family_id) {
		plan = plan->next;
	}

	if (!plan) {
		printf(ORANGE"Family plan not found!"RESET"\n");
		return;
	}

	for (int i = 0; i < plan->customer_count; i++) {
		if (plan->customer_ids[i] == customer_id) {
			for (int j = i; j < plan->customer_count - 1; j++) {
				plan->customer_ids[j] = plan->customer_ids[j + 1];
			}
			plan->customer_count--;
			printf(YELLOW"Customer removed from family plan!"RESET"\n");
			return;
		}
	}

	printf(GREEN"Customer not found in this family plan!"RESET"\n");
}

void listFamilyPlans() {
	if (!family_head) {
		printf(BLUE"No family plans available."RESET"\n");
		return;
	}

	printf(INDIGO"=== Family Plans ==="RESET"\n");
	FamilyPlan* current = family_head;
	while (current) {
		printf(RED"ID: %d | Name: %s | Tariff ID: %d | Members: %d | Discount: %.1f%%"RESET"\n",
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
		printf(ORANGE"Family plan not found!"RESET"\n");
		return;
	}

	printf(RED"=== Family Plan Details ==="RESET"\n");
	printf(ORANGE"ID: %d"RESET"\n", plan->id);
	printf(YELLOW"Name: %s"RESET"\n", plan->name);
	printf(GREEN"Tariff ID: %d"RESET"\n", plan->tariff_id);
	printf(BLUE"Discount: %.1f%%"RESET"\n", plan->discount_percentage);
	printf(INDIGO"Total Price: %.2f"RESET"\n", calculateFamilyPrice(family_id, tariffList));

	printf(RED"Members (%d):"RESET"\n", plan->customer_count);
	for (int i = 0; i < plan->customer_count; i++) {
		printf(ORANGE"  - Customer ID: %d"RESET"\n", plan->customer_ids[i]);
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
		printf(YELLOW"Family plan not found!"RESET"\n");
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

	printf(GREEN"Family plan deleted successfully!"RESET"\n");
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
			printf(BLUE"Family plan name: "RESET);
			read_line("", name, sizeof(name));
			printf(INDIGO"Tariff ID: "RESET);
			scanf("%d", &tariff_id);
			printf(RED"Discount percentage (0-50): "RESET);
			scanf("%lf", &discount);

			if (discount < 0 || discount > 50) {
				printf(ORANGE"Invalid discount! Using 10%%"RESET"\n");
				discount = 10.0;
			}

			createFamilyPlan(name, tariff_id, discount);
			break;
		}
		case 2: {
			printf(YELLOW"Family plan ID: "RESET);
			scanf("%d", &family_id);
			printf(GREEN"Customer ID to add: "RESET);
			scanf("%d", &customer_id);
			addCustomerToFamily(family_id, customer_id);
			break;
		}
		case 3: {
			printf(BLUE"Family plan ID: "RESET);
			scanf("%d", &family_id);
			printf(INDIGO"Customer ID to remove: "RESET);
			scanf("%d", &customer_id);
			removeCustomerFromFamily(family_id, customer_id);
			break;
		}
		case 4: {
			listFamilyPlans();
			break;
		}
		case 5: {
			printf(RED"Family plan ID: "RESET);
			scanf("%d", &family_id);
			showFamilyDetails(family_id, custList, tariffList);
			break;
		}
		case 6: {
			printf(ORANGE"Family plan ID to delete: "RESET);
			scanf("%d", &family_id);
			deleteFamilyPlan(family_id);
			break;
		}
		case 0: {
			printf(YELLOW"Returning to main menu..."RESET"\n");
			break;
		}
		default: {
			printf(GREEN"Invalid choice!"RESET"\n");
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