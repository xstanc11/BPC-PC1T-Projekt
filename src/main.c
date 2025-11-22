// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "main.h"

#include "customer.h"
#include "tariff.h"
#include "util.h"

/* Hlavní hlavičky seznamů */
// Tariff *tariff_head = NULL;
// Customer *cust_head = NULL;
// FamilyPlan *family_head = NULL;

CustomerList_t *custList = NULL;
TariffList_t *tariffList = NULL;


/* Pomocné globální id generátory */
int next_tariff_id = 1;
int next_customer_id = 1;
int next_family_id = 1;

// /* Najdi tariff podle ID */
// Tariff *find_tariff_by_id(int id) {
//     Tariff *cur = tariff_head;
//     while (cur) {
//         if (cur->id == id) return cur;
//         cur = cur->next;
//     }
//     return NULL;
// }

// /* Přidej tariff do globálního seznamu (na konec) */
// void add_tariff(const char *name, double price) {
//     Tariff *t = malloc(sizeof(Tariff));
//     if (!t) { perror("malloc"); exit(1); }
//     t->id = next_tariff_id++;
//     strncpy(t->name, name, MAX_NAME-1); t->name[MAX_NAME-1] = '\0';
//     t->price = price;
//     t->next = NULL;

//     if (!tariff_head) tariff_head = t;
//     else {
//         Tariff *p = tariff_head;
//         while (p->next) p = p->next;
//         p->next = t;
//     }
//     printf("Tariff pridán (ID=%d).\n", t->id);
// }

// /* Edituj tariff podle ID */
// void edit_tariff(int id) {
//     Tariff *t = find_tariff_by_id(id);
//     if (!t) { printf("Tariff nenalezen.\n"); return; }
//     char buf[LINE_BUF];
//     read_line("Nové jméno (nechat prázdné = beze změny): ", buf, sizeof(buf));
//     if (buf[0]) strncpy(t->name, buf, MAX_NAME-1);
//     read_line("Nová cena (nechat prázdné = beze změny): ", buf, sizeof(buf));
//     if (buf[0]) {
//         double p = atof(buf);
//         t->price = p;
//     }
//     printf("Tariff upraven.\n");
// }

/* Smaž tariff - POZOR: musíme odstranit odkazy u zákazníků i rodinných plánů */
// void delete_tariff(int id) {
//     Tariff *prev = NULL, *cur = tariff_head;
//     while (cur && cur->id != id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Tariff nenalezen.\n"); return; }

//     /* Odstranění odkazů z každého zákazníka */
//     Customer *c = cust_head;
//     while (c) {
//         CustTariffNode *pt = c->tariffs;
//         CustTariffNode *pp = NULL;
//         while (pt) {
//             if (pt->tariff == cur) {
//                 /* smažeme tuto položku */
//                 CustTariffNode *tofree = pt;
//                 if (pp) pp->next = pt->next;
//                 else c->tariffs = pt->next;
//                 pt = pt->next;
//                 free(tofree);
//             } else {
//                 pp = pt;
//                 pt = pt->next;
//             }
//         }
//         c = c->next;
//     }

//     /* Rodinné plány: pokud používají tento tariff, nastavíme tariff=NULL */
//     FamilyPlan *f = family_head;
//     while (f) {
//         if (f->tariff == cur) f->tariff = NULL;
//         f = f->next;
//     }

//     /* Odebrat tariff z listu tariffů */
//     if (!prev) tariff_head = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Tariff smazán (a odkazy odstraněny).\n");
// }

/* Vypiš všechny tariffy */
// void list_tariffs(void) {
//     if (!tariff_head) { printf("Žádné tariffy.\n"); return; }
//     printf("Seznam tariffů:\n");
//     Tariff *t = tariff_head;
//     while (t) {
//         printf("  ID=%d | %s | %.2f\n", t->id, t->name, t->price);
//         t = t->next;
//     }
// }

/* Smazat zákazníka a jeho přiřazené položky */
// void delete_customer(int id) {
//     Customer *prev = NULL, *cur = cust_head;
//     while (cur && cur->id != id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Zákazník nenalezen.\n"); return; }

//     /* uvolnit seznam tariffů zákazníka */
//     CustTariffNode *ct = cur->tariffs;
//     while (ct) {
//         CustTariffNode *tmp = ct;
//         ct = ct->next;
//         free(tmp);
//     }

//     /* odstranit z rodinných plánů, pokud člen */
//     FamilyPlan *f = family_head;
//     while (f) {
//         for (int i = 0; i < f->member_count; ++i) {
//             if (f->members[i] == cur) {
//                 /* shift left */
//                 for (int j = i; j + 1 < f->member_count; ++j)
//                     f->members[j] = f->members[j+1];
//                 f->member_count--;
//                 if (f->member_count == 0) {
//                     free(f->members);
//                     f->members = NULL;
//                 } else {
//                     f->members = realloc(f->members, sizeof(Customer*) * f->member_count);
//                 }
//                 break;
//             }
//         }
//         f = f->next;
//     }

//     if (!prev) cust_head = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Zákazník smazán.\n");
// }

/* Přiřadit existující tariff zákazníkovi */
// void assign_tariff_to_customer(int cust_id, int tariff_id) {
//     Customer *c = find_customer_by_id(cust_id);
//     Tariff *t = find_tariff_by_id(tariff_id);
//     if (!c) { printf("Zákazník nenalezen.\n"); return; }
//     if (!t) { printf("Tariff nenalezen.\n"); return; }
//     /* zkontrolovat duplicitu */
//     CustTariffNode *ct = c->tariffs;
//     while (ct) {
//         if (ct->tariff == t) { printf("Tariff již přiřazen.\n"); return; }
//         ct = ct->next;
//     }
//     CustTariffNode *node = malloc(sizeof(CustTariffNode));
//     if (!node) { perror("malloc"); exit(1); }
//     node->tariff = t;
//     node->next = NULL;
//     if (!c->tariffs) c->tariffs = node;
//     else {
//         ct = c->tariffs;
//         while (ct->next) ct = ct->next;
//         ct->next = node;
//     }
//     printf("Tariff přiřazen.\n");
// }

/* Odebrat tariff ze zákazníka podle ID tariffu */
// void remove_tariff_from_customer(int cust_id, int tariff_id) {
//     Customer *c = find_customer_by_id(cust_id);
//     if (!c) { printf("Zákazník nenalezen.\n"); return; }
//     CustTariffNode *cur = c->tariffs, *prev = NULL;
//     while (cur && cur->tariff->id != tariff_id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Tariff u zákazníka nenalezen.\n"); return; }
//     if (!prev) c->tariffs = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Tariff odebrán od zákazníka.\n");
// }

/* --------- Rodinné plány (bonus) --------- */

// /* Vytvořit rodinný plán s daným tariffem (nutno mít existující tariff) */
// void create_family_plan(const char *name, int tariff_id) {
//     Tariff *t = find_tariff_by_id(tariff_id);
//     if (!t) {
//         printf("Tariff nenalezen.\n");
//         return;
//     }
//     FamilyPlan *f = malloc(sizeof(FamilyPlan));
//     if (!f) { perror("malloc"); exit(1); }
//     f->id = next_family_id++;
//     strncpy(f->name, name, MAX_NAME-1); f->name[MAX_NAME-1] = '\0';
//     f->tariff = t;
//     f->members = NULL;
//     f->member_count = 0;
//     f->next = NULL;

//     if (!family_head) family_head = f;
//     else {
//         FamilyPlan *p = family_head;
//         while (p->next) p = p->next;
//         p->next = f;
//     }
//     printf("Rodinný plán vytvořen (ID=%d).\n", f->id);
// }

// /* Přidat člena do rodinného plánu */
// void family_add_member(int family_id, int cust_id) {
//     FamilyPlan *f = family_head;
//     while (f && f->id != family_id) f = f->next;
//     if (!f) { printf("Rodinný plán nenalezen.\n"); return; }
//     Customer *c = find_customer_by_id(cust_id);
//     if (!c) { printf("Zákazník nenalezen.\n"); return; }
//     /* zkontrolovat duplicitu */
//     for (int i = 0; i < f->member_count; ++i) if (f->members[i] == c) {
//         printf("Zákazník je již členem plánu.\n"); return;
//     }
//     f->members = realloc(f->members, sizeof(Customer*) * (f->member_count + 1));
//     if (!f->members) { perror("realloc"); exit(1); }
//     f->members[f->member_count] = c;
//     f->member_count++;
//     printf("Člen přidán do rodinného plánu.\n");
// }

/* Odebrat člena z rodinného plánu */
// void family_remove_member(int family_id, int cust_id) {
//     FamilyPlan *f = family_head;
//     while (f && f->id != family_id) f = f->next;
//     if (!f) { printf("Rodinný plán nenalezen.\n"); return; }
//     int idx = -1;
//     for (int i = 0; i < f->member_count; ++i) {
//         if (f->members[i]->id == cust_id) { idx = i; break; }
//     }
//     if (idx == -1) { printf("Člen v plánu nenalezen.\n"); return; }
//     for (int j = idx; j + 1 < f->member_count; ++j) f->members[j] = f->members[j+1];
//     f->member_count--;
//     if (f->member_count == 0) {
//         free(f->members);
//         f->members = NULL;
//     } else {
//         f->members = realloc(f->members, sizeof(Customer*) * f->member_count);
//     }
//     printf("Člen odstraněn.\n");
// }

/* Vypiš rodinné plány */
// void list_family_plans(void) {
//     if (!family_head) { printf("Žádné rodinné plány.\n"); return; }
//     FamilyPlan *f = family_head;
//     while (f) {
//         printf("ID=%d | %s | Tariff: %s (ID=%d)\n", f->id, f->name,
//                f->tariff ? f->tariff->name : "(žádný)", f->tariff ? f->tariff->id : -1);
//         printf("  Členové (%d):\n", f->member_count);
//         for (int i = 0; i < f->member_count; ++i) {
//             Customer *c = f->members[i];
//             printf("    - ID=%d %s\n", c->id, c->name);
//         }
//         f = f->next;
//     }
// }

/* Smazat rodinný plán */
// void delete_family_plan(int family_id) {
//     FamilyPlan *prev = NULL, *cur = family_head;
//     while (cur && cur->id != family_id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Rodinný plán nenalezen.\n"); return; }
//     if (cur->members) free(cur->members);
//     if (!prev) family_head = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Rodinný plán smazán.\n");
// }

/* --------- Menu a uživatelské rozhraní --------- */

void show_main_menu() {
    printf("\n=== Správa zákazníků mobilního operátora ===\n");
    printf("1) Správa tariffů\n");
    printf("2) Správa zákazníků\n");
    printf("3) Vyhledávání\n");
    printf("4) Řazení zákazníků\n");
    printf("5) Rodinné plány (bonus)\n");
    printf("0) Konec\n");
    printf("Vyberte volbu: ");
}

/* Submenu tariffů */
// void menu_tariffy() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Tariffy ---\n");
//         printf("1) Přidat tariff\n");
//         printf("2) Upravit tariff\n");
//         printf("3) Smazat tariff\n");
//         printf("4) Vypsat tariffy\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) {
//             char name[MAX_NAME], pricebuf[LINE_BUF];
//             read_line("Jméno tariffu: ", name, sizeof(name));
//             read_line("Cena: ", pricebuf, sizeof(pricebuf));
//             double price = atof(pricebuf);
//             add_tariff(name, price);
//         } else if (opt == 2) {
//             read_line("ID tariffu k úpravě: ", buf, sizeof(buf));
//             edit_tariff(atoi(buf));
//         } else if (opt == 3) {
//             read_line("ID tariffu k smazání: ", buf, sizeof(buf));
//             delete_tariff(atoi(buf));
//         } else if (opt == 4) {
//             list_tariffs();
//         } else printf("Neplatná volba.\n");
//     }
// }

/* Submenu zákazníků */
// void menu_zakaznici() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Zákazníci ---\n");
//         printf("1) Přidat zákazníka\n");
//         printf("2) Upravit zákazníka\n");
//         printf("3) Smazat zákazníka\n");
//         printf("4) Vypsat zákazníky\n");
//         printf("5) Přiřadit tariff zákazníkovi\n");
//         printf("6) Odebrat tariff od zákazníka\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) {
//             char name[MAX_NAME], phone[MAX_PHONE];
//             read_line("Jméno: ", name, sizeof(name));
//             read_line("Telefon: ", phone, sizeof(phone));
//             add_customer(name, phone);
//         } else if (opt == 2) {
//             read_line("ID zákazníka k úpravě: ", buf, sizeof(buf));
//             edit_customer(atoi(buf));
//         } else if (opt == 3) {
//             read_line("ID zákazníka k smazání: ", buf, sizeof(buf));
//             delete_customer(atoi(buf));
//         } else if (opt == 4) {
//             list_customers();
//         } else if (opt == 5) {
//             read_line("ID zákazníka: ", buf, sizeof(buf));
//             int cid = atoi(buf);
//             read_line("ID tariffu: ", buf, sizeof(buf));
//             int tid = atoi(buf);
//             assign_tariff_to_customer(cid, tid);
//         } else if (opt == 6) {
//             read_line("ID zákazníka: ", buf, sizeof(buf));
//             int cid = atoi(buf);
//             read_line("ID tariffu k odebrání: ", buf, sizeof(buf));
//             int tid = atoi(buf);
//             remove_tariff_from_customer(cid, tid);
//         } else printf("Neplatná volba.\n");
//     }
// }

/* Submenu vyhledávání */
// void menu_search() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Vyhledávání ---\n");
//         printf("1) Hledat podle jména\n");
//         printf("2) Hledat podle telefonu\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) {
//             read_line("Část jména: ", buf, sizeof(buf));
//             search_customers_by_name(buf);
//         } else if (opt == 2) {
//             read_line("Část telefonu: ", buf, sizeof(buf));
//             search_customers_by_phone(buf);
//         } else printf("Neplatná volba.\n");
//     }
// }

// /* Submenu řazení */
// void menu_sort() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Řazení ---\n");
//         printf("1) Řadit dle abecedy (jméno)\n");
//         printf("2) Řadit dle tariffů (název prvního tariffu)\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) sort_customers_by_name();
//         else if (opt == 2) sort_customers_by_tariff();
//         else printf("Neplatná volba.\n");
//     }
// }

// /* Submenu rodinné plány */
// void menu_family() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Rodinné plány ---\n");
//         printf("1) Vytvořit rodinný plán\n");
//         printf("2) Přidat člena do plánu\n");
//         printf("3) Odebrat člena z plánu\n");
//         printf("4) Vypsat plány\n");
//         printf("5) Smazat plán\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) {
//             char name[MAX_NAME];
//             read_line("Jméno plánu: ", name, sizeof(name));
//             read_line("ID tariffu pro plán: ", buf, sizeof(buf));
//             int tid = atoi(buf);
//             create_family_plan(name, tid);
//         } else if (opt == 2) {
//             read_line("ID plánu: ", buf, sizeof(buf));
//             int fid = atoi(buf);
//             read_line("ID zákazníka: ", buf, sizeof(buf));
//             int cid = atoi(buf);
//             family_add_member(fid, cid);
//         } else if (opt == 3) {
//             read_line("ID plánu: ", buf, sizeof(buf));
//             int fid = atoi(buf);
//             read_line("ID zákazníka: ", buf, sizeof(buf));
//             int cid = atoi(buf);
//             family_remove_member(fid, cid);
//         } else if (opt == 4) {
//             list_family_plans();
//         } else if (opt == 5) {
//             read_line("ID plánu k smazání: ", buf, sizeof(buf));
//             delete_family_plan(atoi(buf));
//         } else printf("Neplatná volba.\n");
//     }
// }

/* Inicializace s pár demo tariffy pro snazší testování */
void seed_demo_data(void) {
    TLInsert(-1, "Rodinný 50 GB", 29.99, tariffList);
    TLInsert(-1, "Neomezené volání", 19.99, tariffList);
    TLInsert(-1, "Základní 100 MB", 5.99, tariffList);
    CLInsert(-1, "Jan Novak", "777111222", NULL, custList);
    CLInsert(-1, "Jan Novek", "777111225", NULL, custList);
    CLInsert(-1, "Jan Aovak", "777111221", NULL, custList);
    CLInsert(-1, "Jane Novak", "777111223", NULL, custList);
}

/* Uvolnit vše při ukončení */
// void free_all(void) {
//     /* Tariffy */
//     Tariff *t = tariff_head;
//     while (t) { Tariff *tmp = t; t = t->next; free(tmp); }
//     /* Zákazníci (a jejich tariffové uzly) */
//     Customer *c = cust_head;
//     while (c) {
//         CustTariffNode *ct = c->tariffs;
//         while (ct) { CustTariffNode *tmp = ct; ct = ct->next; free(tmp); }
//         Customer *tmpc = c; c = c->next; free(tmpc);
//     }
//     /* Rodinné plány */
//     FamilyPlan *f = family_head;
//     while (f) {
//         if (f->members) free(f->members);
//         FamilyPlan *tmp = f; f = f->next; free(tmp);
//     }
// }

int main(void) {
    tariffList = TLInit();
    custList = CLInit();
    seed_demo_data(); /* přidej pár položek pro rychlé testování */
    char buf[LINE_BUF];
    // while (1) {
    //     // show_main_menu();
    //     if (!fgets(buf, sizeof(buf), stdin)) break;
    //     int opt = atoi(buf);
    //     if (opt == 0) break;
    //     // if (opt == 1) menu_tariffy();
    //     // else if (opt == 2) menu_zakaznici();
    //     // else if (opt == 3) menu_search();
    //     // else if (opt == 4) menu_sort();
    //     // else if (opt == 5) menu_family();
    //     else printf("Neplatná volba.\n");
    // }
    // free_all();

    // printf("PRE\n\n");
    // printf("===TARIFS===\n");
    // TLPrint(tariffList);
    // printf("===CUSTOMERS===\n");
    // CLPrint(custList);
    TLEdit(1, NULL, 30, tariffList);
    TLEdit(0, "penis", 69, tariffList);
    CLEdit(0, NULL, NULL, "4124123", custList);
    CLEdit(1, NULL, "Kokotik", NULL, custList);
    CLEdit(2, "PELE", NULL, NULL, custList);
    assignTariff(2, 0, tariffList, custList);
    assignTariff(0, 0, tariffList, custList);
    // printf("AFTER\n\n");
    printf("===TARIFS===\n");
    TLPrint(tariffList);
    // printf("===CUSTOMERS===\n");
    // CLPrint(custList);
    printAssignedTariffs(0, custList);
    unassignTariff(2, 0, tariffList, custList);
    printAssignedTariffs(0, custList);
    CLDispose(custList);
    TLDispose(tariffList);
    free(custList);
    free(tariffList);
    return 0;
}
