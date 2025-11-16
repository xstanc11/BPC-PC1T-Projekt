#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 100
#define MAX_PHONE 32
#define LINE_BUF 256

/* Strukturka pro Tarif (globální seznam tarifů) */
typedef struct Tarif {
    int id;
    char name[MAX_NAME];
    double price;
    struct Tarif *next;
} Tarif;

/* Pro každý zákazník: seznam přiřazených tarifů (ukazatele na Tarif) */
typedef struct CustTarifNode {
    Tarif *tarif; /* ukazatel na tarif v globálním seznamu */
    struct CustTarifNode *next;
} CustTarifNode;

/* Zákazník v lineárně vázaném seznamu */
typedef struct Customer {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    CustTarifNode *tarifs; /* linked list tarifů přiřazených tomuto zákazníkovi */
    struct Customer *next;
} Customer;

/* Rodinný tarif: kolekce zákazníků sdílející tarif (bonus) */
typedef struct FamilyPlan {
    int id;
    char name[MAX_NAME]; /* název rodinného plánu */
    Tarif *tarif; /* tarif, který sdílí */
    Customer **members; /* dynamické pole ukazatelů na členy (jednodušší implementace) */
    int member_count;
    struct FamilyPlan *next;
} FamilyPlan;

/* Hlavní hlavičky seznamů */
Tarif *tarif_head = NULL;
Customer *cust_head = NULL;
FamilyPlan *family_head = NULL;

/* Pomocné globální id generátory */
int next_tarif_id = 1;
int next_customer_id = 1;
int next_family_id = 1;

/* --------- Utility funkce --------- */
void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_line(const char *prompt, char *buf, size_t sz) {
    if (prompt) printf("%s", prompt);
    if (fgets(buf, (int)sz, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t ln = strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n') buf[ln-1] = '\0';
}

/* vrátí 1 pokud a<b alphabeticky (case-insensitive) */
int str_ci_lt(const char *a, const char *b) {
    for (;; a++, b++) {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca < cb) return 1;
        if (ca > cb) return 0;
        if (ca == '\0') return 0;
    }
}

/* Najdi tarif podle ID */
Tarif *find_tarif_by_id(int id) {
    Tarif *cur = tarif_head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* Najdi zákazníka podle ID */
Customer *find_customer_by_id(int id) {
    Customer *cur = cust_head;
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}


/* safe strstr case-insensitive */
char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++; n++;
        }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}

/* Najdi zákazníka podle jména (vrátí první, case-insensitive obsahuje) */
Customer *find_customer_by_name(const char *name) {
    Customer *cur = cust_head;
    while (cur) {
        if (strcasestr(cur->name, name) != NULL) return cur;
        cur = cur->next;
    }
    return NULL;
}


/* Přidej tarif do globálního seznamu (na konec) */
void add_tarif(const char *name, double price) {
    Tarif *t = malloc(sizeof(Tarif));
    if (!t) { perror("malloc"); exit(1); }
    t->id = next_tarif_id++;
    strncpy(t->name, name, MAX_NAME-1); t->name[MAX_NAME-1] = '\0';
    t->price = price;
    t->next = NULL;

    if (!tarif_head) tarif_head = t;
    else {
        Tarif *p = tarif_head;
        while (p->next) p = p->next;
        p->next = t;
    }
    printf("Tarif pridán (ID=%d).\n", t->id);
}

/* Edituj tarif podle ID */
void edit_tarif(int id) {
    Tarif *t = find_tarif_by_id(id);
    if (!t) { printf("Tarif nenalezen.\n"); return; }
    char buf[LINE_BUF];
    read_line("Nové jméno (nechat prázdné = beze změny): ", buf, sizeof(buf));
    if (buf[0]) strncpy(t->name, buf, MAX_NAME-1);
    read_line("Nová cena (nechat prázdné = beze změny): ", buf, sizeof(buf));
    if (buf[0]) {
        double p = atof(buf);
        t->price = p;
    }
    printf("Tarif upraven.\n");
}

/* Smaž tarif - POZOR: musíme odstranit odkazy u zákazníků i rodinných plánů */
void delete_tarif(int id) {
    Tarif *prev = NULL, *cur = tarif_head;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Tarif nenalezen.\n"); return; }

    /* Odstranění odkazů z každého zákazníka */
    Customer *c = cust_head;
    while (c) {
        CustTarifNode *pt = c->tarifs;
        CustTarifNode *pp = NULL;
        while (pt) {
            if (pt->tarif == cur) {
                /* smažeme tuto položku */
                CustTarifNode *tofree = pt;
                if (pp) pp->next = pt->next;
                else c->tarifs = pt->next;
                pt = pt->next;
                free(tofree);
            } else {
                pp = pt;
                pt = pt->next;
            }
        }
        c = c->next;
    }

    /* Rodinné plány: pokud používají tento tarif, nastavíme tarif=NULL */
    FamilyPlan *f = family_head;
    while (f) {
        if (f->tarif == cur) f->tarif = NULL;
        f = f->next;
    }

    /* Odebrat tarif z listu tarifů */
    if (!prev) tarif_head = cur->next;
    else prev->next = cur->next;
    free(cur);
    printf("Tarif smazán (a odkazy odstraněny).\n");
}

/* Vypiš všechny tarify */
void list_tarifs(void) {
    if (!tarif_head) { printf("Žádné tarify.\n"); return; }
    printf("Seznam tarifů:\n");
    Tarif *t = tarif_head;
    while (t) {
        printf("  ID=%d | %s | %.2f\n", t->id, t->name, t->price);
        t = t->next;
    }
}

/* --------- Zákazníci --------- */

/* Přidej zákazníka (na konec) */
void add_customer(const char *name, const char *phone) {
    Customer *c = malloc(sizeof(Customer));
    if (!c) { perror("malloc"); exit(1); }
    c->id = next_customer_id++;
    strncpy(c->name, name, MAX_NAME-1); c->name[MAX_NAME-1] = '\0';
    strncpy(c->phone, phone, MAX_PHONE-1); c->phone[MAX_PHONE-1] = '\0';
    c->tarifs = NULL;
    c->next = NULL;

    if (!cust_head) cust_head = c;
    else {
        Customer *p = cust_head;
        while (p->next) p = p->next;
        p->next = c;
    }
    printf("Zákazník přidán (ID=%d).\n", c->id);
}

/* Edit zákazníka */
void edit_customer(int id) {
    Customer *c = find_customer_by_id(id);
    if (!c) { printf("Zákazník nenalezen.\n"); return; }
    char buf[LINE_BUF];
    read_line("Nové jméno (nechat prázdné = beze změny): ", buf, sizeof(buf));
    if (buf[0]) strncpy(c->name, buf, MAX_NAME-1);
    read_line("Nové telefonní číslo (nechat prázdné = beze změny): ", buf, sizeof(buf));
    if (buf[0]) strncpy(c->phone, buf, MAX_PHONE-1);
    printf("Zákazník upraven.\n");
}

/* Smazat zákazníka a jeho přiřazené položky */
void delete_customer(int id) {
    Customer *prev = NULL, *cur = cust_head;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Zákazník nenalezen.\n"); return; }

    /* uvolnit seznam tarifů zákazníka */
    CustTarifNode *ct = cur->tarifs;
    while (ct) {
        CustTarifNode *tmp = ct;
        ct = ct->next;
        free(tmp);
    }

    /* odstranit z rodinných plánů, pokud člen */
    FamilyPlan *f = family_head;
    while (f) {
        for (int i = 0; i < f->member_count; ++i) {
            if (f->members[i] == cur) {
                /* shift left */
                for (int j = i; j + 1 < f->member_count; ++j)
                    f->members[j] = f->members[j+1];
                f->member_count--;
                if (f->member_count == 0) {
                    free(f->members);
                    f->members = NULL;
                } else {
                    f->members = realloc(f->members, sizeof(Customer*) * f->member_count);
                }
                break;
            }
        }
        f = f->next;
    }

    if (!prev) cust_head = cur->next;
    else prev->next = cur->next;
    free(cur);
    printf("Zákazník smazán.\n");
}

/* Vypiš všechny zákazníky */
void list_customers(void) {
    if (!cust_head) { printf("Žádní zákazníci.\n"); return; }
    Customer *c = cust_head;
    printf("Seznam zákazníků:\n");
    while (c) {
        printf("  ID=%d | %s | %s\n", c->id, c->name, c->phone);
        /* výpis tarifů zákazníka */
        CustTarifNode *ct = c->tarifs;
        if (!ct) printf("    (žádné tarify)\n");
        else {
            printf("    Tarify:\n");
            while (ct) {
                if (ct->tarif)
                    printf("      - ID=%d %s (%.2f)\n", ct->tarif->id, ct->tarif->name, ct->tarif->price);
                else
                    printf("      - (smazaný tarif)\n");
                ct = ct->next;
            }
        }
        c = c->next;
    }
}

/* Přiřadit existující tarif zákazníkovi */
void assign_tarif_to_customer(int cust_id, int tarif_id) {
    Customer *c = find_customer_by_id(cust_id);
    Tarif *t = find_tarif_by_id(tarif_id);
    if (!c) { printf("Zákazník nenalezen.\n"); return; }
    if (!t) { printf("Tarif nenalezen.\n"); return; }
    /* zkontrolovat duplicitu */
    CustTarifNode *ct = c->tarifs;
    while (ct) {
        if (ct->tarif == t) { printf("Tarif již přiřazen.\n"); return; }
        ct = ct->next;
    }
    CustTarifNode *node = malloc(sizeof(CustTarifNode));
    if (!node) { perror("malloc"); exit(1); }
    node->tarif = t;
    node->next = NULL;
    if (!c->tarifs) c->tarifs = node;
    else {
        ct = c->tarifs;
        while (ct->next) ct = ct->next;
        ct->next = node;
    }
    printf("Tarif přiřazen.\n");
}

/* Odebrat tarif ze zákazníka podle ID tarifu */
void remove_tarif_from_customer(int cust_id, int tarif_id) {
    Customer *c = find_customer_by_id(cust_id);
    if (!c) { printf("Zákazník nenalezen.\n"); return; }
    CustTarifNode *cur = c->tarifs, *prev = NULL;
    while (cur && cur->tarif->id != tarif_id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Tarif u zákazníka nenalezen.\n"); return; }
    if (!prev) c->tarifs = cur->next;
    else prev->next = cur->next;
    free(cur);
    printf("Tarif odebrán od zákazníka.\n");
}

/* Hledání zákazníků podle části jména (case-insensitive) */
void search_customers_by_name(const char *pattern) {
    Customer *c = cust_head;
    int found = 0;
    while (c) {
        if (strcasestr(c->name, pattern)) {
            printf("  ID=%d | %s | %s\n", c->id, c->name, c->phone);
            found = 1;
        }
        c = c->next;
    }
    if (!found) printf("Žádní zákazníci neodpovídají.\n");
}

/* Hledání zákazníka podle telefonu (částečná shoda) */
void search_customers_by_phone(const char *pattern) {
    Customer *c = cust_head;
    int found = 0;
    while (c) {
        if (strcasestr(c->phone, pattern)) {
            printf("  ID=%d | %s | %s\n", c->id, c->name, c->phone);
            found = 1;
        }
        c = c->next;
    }
    if (!found) printf("Žádní zákazníci neodpovídají.\n");
}

/* --------- Řazení zákazníků (merge sort pro linked list) --------- */

/* pomocná: get middle */
Customer* get_middle(Customer* head) {
    if (!head) return head;
    Customer *slow = head, *fast = head->next;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow;
}

/* Porovnání zákazníků podle abecedy jména */
int customer_cmp_name(Customer *a, Customer *b) {
    return strcasestr(a->name, b->name) <= 0;
}

/* Porovnání podle jména prvního tarifu (pokud žádný -> na konec) */
int customer_cmp_by_tarif(Customer *a, Customer *b) {
    const char *an = NULL, *bn = NULL;
    if (a->tarifs && a->tarifs->tarif) an = a->tarifs->tarif->name;
    if (b->tarifs && b->tarifs->tarif) bn = b->tarifs->tarif->name;
    if (!an && !bn) return 1; /* rovnocenné, a první půjde před b */
    if (!an) return 0; /* a bez tarifu -> považovat větší => jde za b */
    if (!bn) return 1;
    return strcasestr(an, bn) <= 0;
}

Customer* sorted_merge(Customer* a, Customer* b, int (*cmp)(Customer*, Customer*)) {
    if (!a) return b;
    if (!b) return a;
    Customer *result = NULL;
    if (cmp(a,b)) {
        result = a;
        result->next = sorted_merge(a->next, b, cmp);
    } else {
        result = b;
        result->next = sorted_merge(a, b->next, cmp);
    }
    return result;
}

/* Obecný mergesort; cmp by mělo vracet 1 pokud a<=b */
Customer* merge_sort_customers(Customer* head, int (*cmp)(Customer*, Customer*)) {
    if (!head || !head->next) return head;
    Customer *mid = get_middle(head);
    Customer *second = mid->next;
    mid->next = NULL;
    Customer *left = merge_sort_customers(head, cmp);
    Customer *right = merge_sort_customers(second, cmp);
    return sorted_merge(left, right, cmp);
}

/* Wrapper pro řazení podle jména */
void sort_customers_by_name(void) {
    cust_head = merge_sort_customers(cust_head, customer_cmp_name);
    printf("Zákazníci seřazeni dle abecedy.\n");
}

/* Wrapper pro řazení dle tarifů (podle názvu prvního tarifu) */
void sort_customers_by_tarif(void) {
    cust_head = merge_sort_customers(cust_head, customer_cmp_by_tarif);
    printf("Zákazníci seřazeni dle tarifů (podle názvu prvního tarifu).\n");
}

/* --------- Rodinné plány (bonus) --------- */

/* Vytvořit rodinný plán s daným tarifem (nutno mít existující tarif) */
void create_family_plan(const char *name, int tarif_id) {
    Tarif *t = find_tarif_by_id(tarif_id);
    if (!t) {
        printf("Tarif nenalezen.\n");
        return;
    }
    FamilyPlan *f = malloc(sizeof(FamilyPlan));
    if (!f) { perror("malloc"); exit(1); }
    f->id = next_family_id++;
    strncpy(f->name, name, MAX_NAME-1); f->name[MAX_NAME-1] = '\0';
    f->tarif = t;
    f->members = NULL;
    f->member_count = 0;
    f->next = NULL;

    if (!family_head) family_head = f;
    else {
        FamilyPlan *p = family_head;
        while (p->next) p = p->next;
        p->next = f;
    }
    printf("Rodinný plán vytvořen (ID=%d).\n", f->id);
}

/* Přidat člena do rodinného plánu */
void family_add_member(int family_id, int cust_id) {
    FamilyPlan *f = family_head;
    while (f && f->id != family_id) f = f->next;
    if (!f) { printf("Rodinný plán nenalezen.\n"); return; }
    Customer *c = find_customer_by_id(cust_id);
    if (!c) { printf("Zákazník nenalezen.\n"); return; }
    /* zkontrolovat duplicitu */
    for (int i = 0; i < f->member_count; ++i) if (f->members[i] == c) {
        printf("Zákazník je již členem plánu.\n"); return;
    }
    f->members = realloc(f->members, sizeof(Customer*) * (f->member_count + 1));
    if (!f->members) { perror("realloc"); exit(1); }
    f->members[f->member_count] = c;
    f->member_count++;
    printf("Člen přidán do rodinného plánu.\n");
}

/* Odebrat člena z rodinného plánu */
void family_remove_member(int family_id, int cust_id) {
    FamilyPlan *f = family_head;
    while (f && f->id != family_id) f = f->next;
    if (!f) { printf("Rodinný plán nenalezen.\n"); return; }
    int idx = -1;
    for (int i = 0; i < f->member_count; ++i) {
        if (f->members[i]->id == cust_id) { idx = i; break; }
    }
    if (idx == -1) { printf("Člen v plánu nenalezen.\n"); return; }
    for (int j = idx; j + 1 < f->member_count; ++j) f->members[j] = f->members[j+1];
    f->member_count--;
    if (f->member_count == 0) {
        free(f->members);
        f->members = NULL;
    } else {
        f->members = realloc(f->members, sizeof(Customer*) * f->member_count);
    }
    printf("Člen odstraněn.\n");
}

/* Vypiš rodinné plány */
void list_family_plans(void) {
    if (!family_head) { printf("Žádné rodinné plány.\n"); return; }
    FamilyPlan *f = family_head;
    while (f) {
        printf("ID=%d | %s | Tarif: %s (ID=%d)\n", f->id, f->name,
               f->tarif ? f->tarif->name : "(žádný)", f->tarif ? f->tarif->id : -1);
        printf("  Členové (%d):\n", f->member_count);
        for (int i = 0; i < f->member_count; ++i) {
            Customer *c = f->members[i];
            printf("    - ID=%d %s\n", c->id, c->name);
        }
        f = f->next;
    }
}

/* Smazat rodinný plán */
void delete_family_plan(int family_id) {
    FamilyPlan *prev = NULL, *cur = family_head;
    while (cur && cur->id != family_id) { prev = cur; cur = cur->next; }
    if (!cur) { printf("Rodinný plán nenalezen.\n"); return; }
    if (cur->members) free(cur->members);
    if (!prev) family_head = cur->next;
    else prev->next = cur->next;
    free(cur);
    printf("Rodinný plán smazán.\n");
}

/* --------- Menu a uživatelské rozhraní --------- */

void show_main_menu() {
    printf("\n=== Správa zákazníků mobilního operátora ===\n");
    printf("1) Správa tarifů\n");
    printf("2) Správa zákazníků\n");
    printf("3) Vyhledávání\n");
    printf("4) Řazení zákazníků\n");
    printf("5) Rodinné plány (bonus)\n");
    printf("0) Konec\n");
    printf("Vyberte volbu: ");
}

/* Submenu tarifů */
void menu_tarify() {
    char buf[LINE_BUF];
    while (1) {
        printf("\n--- Tarify ---\n");
        printf("1) Přidat tarif\n");
        printf("2) Upravit tarif\n");
        printf("3) Smazat tarif\n");
        printf("4) Vypsat tarify\n");
        printf("0) Zpět\n");
        printf("Volba: ");
        read_line(NULL, buf, sizeof(buf));
        if (!buf[0]) continue;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) {
            char name[MAX_NAME], pricebuf[LINE_BUF];
            read_line("Jméno tarifu: ", name, sizeof(name));
            read_line("Cena: ", pricebuf, sizeof(pricebuf));
            double price = atof(pricebuf);
            add_tarif(name, price);
        } else if (opt == 2) {
            read_line("ID tarifu k úpravě: ", buf, sizeof(buf));
            edit_tarif(atoi(buf));
        } else if (opt == 3) {
            read_line("ID tarifu k smazání: ", buf, sizeof(buf));
            delete_tarif(atoi(buf));
        } else if (opt == 4) {
            list_tarifs();
        } else printf("Neplatná volba.\n");
    }
}

/* Submenu zákazníků */
void menu_zakaznici() {
    char buf[LINE_BUF];
    while (1) {
        printf("\n--- Zákazníci ---\n");
        printf("1) Přidat zákazníka\n");
        printf("2) Upravit zákazníka\n");
        printf("3) Smazat zákazníka\n");
        printf("4) Vypsat zákazníky\n");
        printf("5) Přiřadit tarif zákazníkovi\n");
        printf("6) Odebrat tarif od zákazníka\n");
        printf("0) Zpět\n");
        printf("Volba: ");
        read_line(NULL, buf, sizeof(buf));
        if (!buf[0]) continue;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) {
            char name[MAX_NAME], phone[MAX_PHONE];
            read_line("Jméno: ", name, sizeof(name));
            read_line("Telefon: ", phone, sizeof(phone));
            add_customer(name, phone);
        } else if (opt == 2) {
            read_line("ID zákazníka k úpravě: ", buf, sizeof(buf));
            edit_customer(atoi(buf));
        } else if (opt == 3) {
            read_line("ID zákazníka k smazání: ", buf, sizeof(buf));
            delete_customer(atoi(buf));
        } else if (opt == 4) {
            list_customers();
        } else if (opt == 5) {
            read_line("ID zákazníka: ", buf, sizeof(buf));
            int cid = atoi(buf);
            read_line("ID tarifu: ", buf, sizeof(buf));
            int tid = atoi(buf);
            assign_tarif_to_customer(cid, tid);
        } else if (opt == 6) {
            read_line("ID zákazníka: ", buf, sizeof(buf));
            int cid = atoi(buf);
            read_line("ID tarifu k odebrání: ", buf, sizeof(buf));
            int tid = atoi(buf);
            remove_tarif_from_customer(cid, tid);
        } else printf("Neplatná volba.\n");
    }
}

/* Submenu vyhledávání */
void menu_search() {
    char buf[LINE_BUF];
    while (1) {
        printf("\n--- Vyhledávání ---\n");
        printf("1) Hledat podle jména\n");
        printf("2) Hledat podle telefonu\n");
        printf("0) Zpět\n");
        printf("Volba: ");
        read_line(NULL, buf, sizeof(buf));
        if (!buf[0]) continue;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) {
            read_line("Část jména: ", buf, sizeof(buf));
            search_customers_by_name(buf);
        } else if (opt == 2) {
            read_line("Část telefonu: ", buf, sizeof(buf));
            search_customers_by_phone(buf);
        } else printf("Neplatná volba.\n");
    }
}

/* Submenu řazení */
void menu_sort() {
    char buf[LINE_BUF];
    while (1) {
        printf("\n--- Řazení ---\n");
        printf("1) Řadit dle abecedy (jméno)\n");
        printf("2) Řadit dle tarifů (název prvního tarifu)\n");
        printf("0) Zpět\n");
        printf("Volba: ");
        read_line(NULL, buf, sizeof(buf));
        if (!buf[0]) continue;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) sort_customers_by_name();
        else if (opt == 2) sort_customers_by_tarif();
        else printf("Neplatná volba.\n");
    }
}

/* Submenu rodinné plány */
void menu_family() {
    char buf[LINE_BUF];
    while (1) {
        printf("\n--- Rodinné plány ---\n");
        printf("1) Vytvořit rodinný plán\n");
        printf("2) Přidat člena do plánu\n");
        printf("3) Odebrat člena z plánu\n");
        printf("4) Vypsat plány\n");
        printf("5) Smazat plán\n");
        printf("0) Zpět\n");
        printf("Volba: ");
        read_line(NULL, buf, sizeof(buf));
        if (!buf[0]) continue;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) {
            char name[MAX_NAME];
            read_line("Jméno plánu: ", name, sizeof(name));
            read_line("ID tarifu pro plán: ", buf, sizeof(buf));
            int tid = atoi(buf);
            create_family_plan(name, tid);
        } else if (opt == 2) {
            read_line("ID plánu: ", buf, sizeof(buf));
            int fid = atoi(buf);
            read_line("ID zákazníka: ", buf, sizeof(buf));
            int cid = atoi(buf);
            family_add_member(fid, cid);
        } else if (opt == 3) {
            read_line("ID plánu: ", buf, sizeof(buf));
            int fid = atoi(buf);
            read_line("ID zákazníka: ", buf, sizeof(buf));
            int cid = atoi(buf);
            family_remove_member(fid, cid);
        } else if (opt == 4) {
            list_family_plans();
        } else if (opt == 5) {
            read_line("ID plánu k smazání: ", buf, sizeof(buf));
            delete_family_plan(atoi(buf));
        } else printf("Neplatná volba.\n");
    }
}

/* Inicializace s pár demo tarify pro snazší testování */
void seed_demo_data(void) {
    add_tarif("Základní 100 MB", 5.99);
    add_tarif("Neomezené volání", 19.99);
    add_tarif("Rodinný 50 GB", 29.99);
    add_customer("Jan Novak", "777111222");
    add_customer("Petr Svoboda", "606333444");
    assign_tarif_to_customer(1, 1);
    assign_tarif_to_customer(1, 2);
    assign_tarif_to_customer(2, 2);
}

/* Uvolnit vše při ukončení */
void free_all(void) {
    /* Tarify */
    Tarif *t = tarif_head;
    while (t) { Tarif *tmp = t; t = t->next; free(tmp); }
    /* Zákazníci (a jejich tarifové uzly) */
    Customer *c = cust_head;
    while (c) {
        CustTarifNode *ct = c->tarifs;
        while (ct) { CustTarifNode *tmp = ct; ct = ct->next; free(tmp); }
        Customer *tmpc = c; c = c->next; free(tmpc);
    }
    /* Rodinné plány */
    FamilyPlan *f = family_head;
    while (f) {
        if (f->members) free(f->members);
        FamilyPlan *tmp = f; f = f->next; free(tmp);
    }
}

int main(void) {
    seed_demo_data(); /* přidej pár položek pro rychlé testování */
    char buf[LINE_BUF];
    while (1) {
        show_main_menu();
        if (!fgets(buf, sizeof(buf), stdin)) break;
        int opt = atoi(buf);
        if (opt == 0) break;
        if (opt == 1) menu_tarify();
        else if (opt == 2) menu_zakaznici();
        else if (opt == 3) menu_search();
        else if (opt == 4) menu_sort();
        else if (opt == 5) menu_family();
        else printf("Neplatná volba.\n");
    }
    free_all();
    printf("Konec programu.\n");
    return 0;
}
