// BPC-PC1T 2025 Project
// @authors Šimon Čada, Rastislav Samuel Stanček

#include "main.h"
#include "util.h"
#include "customer.h"
#include "tarif.h"

/* Hlavní hlavičky seznamů */
// Tarif *tarif_head = NULL;
// Customer *cust_head = NULL;
FamilyPlan *family_head = NULL;

CustomerList_t *custList = NULL;
TarifList_t *tarifList = NULL;


/* Pomocné globální id generátory */
int next_tarif_id = 1;
int next_customer_id = 1;
int next_family_id = 1;

// /* Najdi tarif podle ID */
// Tarif *find_tarif_by_id(int id) {
//     Tarif *cur = tarif_head;
//     while (cur) {
//         if (cur->id == id) return cur;
//         cur = cur->next;
//     }
//     return NULL;
// }

// /* Přidej tarif do globálního seznamu (na konec) */
// void add_tarif(const char *name, double price) {
//     Tarif *t = malloc(sizeof(Tarif));
//     if (!t) { perror("malloc"); exit(1); }
//     t->id = next_tarif_id++;
//     strncpy(t->name, name, MAX_NAME-1); t->name[MAX_NAME-1] = '\0';
//     t->price = price;
//     t->next = NULL;

//     if (!tarif_head) tarif_head = t;
//     else {
//         Tarif *p = tarif_head;
//         while (p->next) p = p->next;
//         p->next = t;
//     }
//     printf("Tarif pridán (ID=%d).\n", t->id);
// }

// /* Edituj tarif podle ID */
// void edit_tarif(int id) {
//     Tarif *t = find_tarif_by_id(id);
//     if (!t) { printf("Tarif nenalezen.\n"); return; }
//     char buf[LINE_BUF];
//     read_line("Nové jméno (nechat prázdné = beze změny): ", buf, sizeof(buf));
//     if (buf[0]) strncpy(t->name, buf, MAX_NAME-1);
//     read_line("Nová cena (nechat prázdné = beze změny): ", buf, sizeof(buf));
//     if (buf[0]) {
//         double p = atof(buf);
//         t->price = p;
//     }
//     printf("Tarif upraven.\n");
// }

/* Smaž tarif - POZOR: musíme odstranit odkazy u zákazníků i rodinných plánů */
// void delete_tarif(int id) {
//     Tarif *prev = NULL, *cur = tarif_head;
//     while (cur && cur->id != id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Tarif nenalezen.\n"); return; }

//     /* Odstranění odkazů z každého zákazníka */
//     Customer *c = cust_head;
//     while (c) {
//         CustTarifNode *pt = c->tarifs;
//         CustTarifNode *pp = NULL;
//         while (pt) {
//             if (pt->tarif == cur) {
//                 /* smažeme tuto položku */
//                 CustTarifNode *tofree = pt;
//                 if (pp) pp->next = pt->next;
//                 else c->tarifs = pt->next;
//                 pt = pt->next;
//                 free(tofree);
//             } else {
//                 pp = pt;
//                 pt = pt->next;
//             }
//         }
//         c = c->next;
//     }

//     /* Rodinné plány: pokud používají tento tarif, nastavíme tarif=NULL */
//     FamilyPlan *f = family_head;
//     while (f) {
//         if (f->tarif == cur) f->tarif = NULL;
//         f = f->next;
//     }

//     /* Odebrat tarif z listu tarifů */
//     if (!prev) tarif_head = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Tarif smazán (a odkazy odstraněny).\n");
// }

/* Vypiš všechny tarify */
// void list_tarifs(void) {
//     if (!tarif_head) { printf("Žádné tarify.\n"); return; }
//     printf("Seznam tarifů:\n");
//     Tarif *t = tarif_head;
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

//     /* uvolnit seznam tarifů zákazníka */
//     CustTarifNode *ct = cur->tarifs;
//     while (ct) {
//         CustTarifNode *tmp = ct;
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

/* Přiřadit existující tarif zákazníkovi */
// void assign_tarif_to_customer(int cust_id, int tarif_id) {
//     Customer *c = find_customer_by_id(cust_id);
//     Tarif *t = find_tarif_by_id(tarif_id);
//     if (!c) { printf("Zákazník nenalezen.\n"); return; }
//     if (!t) { printf("Tarif nenalezen.\n"); return; }
//     /* zkontrolovat duplicitu */
//     CustTarifNode *ct = c->tarifs;
//     while (ct) {
//         if (ct->tarif == t) { printf("Tarif již přiřazen.\n"); return; }
//         ct = ct->next;
//     }
//     CustTarifNode *node = malloc(sizeof(CustTarifNode));
//     if (!node) { perror("malloc"); exit(1); }
//     node->tarif = t;
//     node->next = NULL;
//     if (!c->tarifs) c->tarifs = node;
//     else {
//         ct = c->tarifs;
//         while (ct->next) ct = ct->next;
//         ct->next = node;
//     }
//     printf("Tarif přiřazen.\n");
// }

/* Odebrat tarif ze zákazníka podle ID tarifu */
// void remove_tarif_from_customer(int cust_id, int tarif_id) {
//     Customer *c = find_customer_by_id(cust_id);
//     if (!c) { printf("Zákazník nenalezen.\n"); return; }
//     CustTarifNode *cur = c->tarifs, *prev = NULL;
//     while (cur && cur->tarif->id != tarif_id) { prev = cur; cur = cur->next; }
//     if (!cur) { printf("Tarif u zákazníka nenalezen.\n"); return; }
//     if (!prev) c->tarifs = cur->next;
//     else prev->next = cur->next;
//     free(cur);
//     printf("Tarif odebrán od zákazníka.\n");
// }

/* --------- Rodinné plány (bonus) --------- */

// /* Vytvořit rodinný plán s daným tarifem (nutno mít existující tarif) */
// void create_family_plan(const char *name, int tarif_id) {
//     Tarif *t = find_tarif_by_id(tarif_id);
//     if (!t) {
//         printf("Tarif nenalezen.\n");
//         return;
//     }
//     FamilyPlan *f = malloc(sizeof(FamilyPlan));
//     if (!f) { perror("malloc"); exit(1); }
//     f->id = next_family_id++;
//     strncpy(f->name, name, MAX_NAME-1); f->name[MAX_NAME-1] = '\0';
//     f->tarif = t;
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
//         printf("ID=%d | %s | Tarif: %s (ID=%d)\n", f->id, f->name,
//                f->tarif ? f->tarif->name : "(žádný)", f->tarif ? f->tarif->id : -1);
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
    printf("1) Správa tarifů\n");
    printf("2) Správa zákazníků\n");
    printf("3) Vyhledávání\n");
    printf("4) Řazení zákazníků\n");
    printf("5) Rodinné plány (bonus)\n");
    printf("0) Konec\n");
    printf("Vyberte volbu: ");
}

/* Submenu tarifů */
// void menu_tarify() {
//     char buf[LINE_BUF];
//     while (1) {
//         printf("\n--- Tarify ---\n");
//         printf("1) Přidat tarif\n");
//         printf("2) Upravit tarif\n");
//         printf("3) Smazat tarif\n");
//         printf("4) Vypsat tarify\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) {
//             char name[MAX_NAME], pricebuf[LINE_BUF];
//             read_line("Jméno tarifu: ", name, sizeof(name));
//             read_line("Cena: ", pricebuf, sizeof(pricebuf));
//             double price = atof(pricebuf);
//             add_tarif(name, price);
//         } else if (opt == 2) {
//             read_line("ID tarifu k úpravě: ", buf, sizeof(buf));
//             edit_tarif(atoi(buf));
//         } else if (opt == 3) {
//             read_line("ID tarifu k smazání: ", buf, sizeof(buf));
//             delete_tarif(atoi(buf));
//         } else if (opt == 4) {
//             list_tarifs();
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
//         printf("5) Přiřadit tarif zákazníkovi\n");
//         printf("6) Odebrat tarif od zákazníka\n");
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
//             read_line("ID tarifu: ", buf, sizeof(buf));
//             int tid = atoi(buf);
//             assign_tarif_to_customer(cid, tid);
//         } else if (opt == 6) {
//             read_line("ID zákazníka: ", buf, sizeof(buf));
//             int cid = atoi(buf);
//             read_line("ID tarifu k odebrání: ", buf, sizeof(buf));
//             int tid = atoi(buf);
//             remove_tarif_from_customer(cid, tid);
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
//         printf("2) Řadit dle tarifů (název prvního tarifu)\n");
//         printf("0) Zpět\n");
//         printf("Volba: ");
//         read_line(NULL, buf, sizeof(buf));
//         if (!buf[0]) continue;
//         int opt = atoi(buf);
//         if (opt == 0) break;
//         if (opt == 1) sort_customers_by_name();
//         else if (opt == 2) sort_customers_by_tarif();
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
//             read_line("ID tarifu pro plán: ", buf, sizeof(buf));
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

/* Inicializace s pár demo tarify pro snazší testování */
void seed_demo_data(void) {
    TLInsert(-1, "Rodinný 50 GB", 29.99, tarifList);
    TLInsert(-1, "Neomezené volání", 19.99, tarifList);
    TLInsert(-1, "Základní 100 MB", 5.99, tarifList);
    CLInsert(-1, "Jan Novak", "777111222", custList);
    CLInsert(-1, "Jan Novek", "777111225", custList);
    CLInsert(-1, "Jan Aovak", "777111221", custList);
    CLInsert(-1, "Jane Novak", "777111223", custList);
}

/* Uvolnit vše při ukončení */
// void free_all(void) {
//     /* Tarify */
//     Tarif *t = tarif_head;
//     while (t) { Tarif *tmp = t; t = t->next; free(tmp); }
//     /* Zákazníci (a jejich tarifové uzly) */
//     Customer *c = cust_head;
//     while (c) {
//         CustTarifNode *ct = c->tarifs;
//         while (ct) { CustTarifNode *tmp = ct; ct = ct->next; free(tmp); }
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
    tarifList = malloc(sizeof(TarifList_t));
    TLInit(tarifList);
    custList = malloc(sizeof(CustomerList_t));
    CLInit(custList);
    seed_demo_data(); /* přidej pár položek pro rychlé testování */
    char buf[LINE_BUF];
    // while (1) {
    //     // show_main_menu();
    //     if (!fgets(buf, sizeof(buf), stdin)) break;
    //     int opt = atoi(buf);
    //     if (opt == 0) break;
    //     // if (opt == 1) menu_tarify();
    //     // else if (opt == 2) menu_zakaznici();
    //     // else if (opt == 3) menu_search();
    //     // else if (opt == 4) menu_sort();
    //     // else if (opt == 5) menu_family();
    //     else printf("Neplatná volba.\n");
    // }
    // free_all();
    printf("PRE\n\n");
    printf("===TARIFS===\n");
    TLPrint(tarifList);
    printf("===CUSTOMERS===\n");
    CLPrint(custList);
    TLEdit(1, NULL, 30, tarifList);
    TLEdit(0, "penis", 69, tarifList);
    CLEdit(0, NULL, NULL, "4124123", custList);
    CLEdit(1, NULL, "Kokotik", NULL, custList);
    CLEdit(2, "PELE", NULL, NULL, custList);
    printf("AFTER\n\n");
    printf("===TARIFS===\n");
    TLPrint(tarifList);
    printf("===CUSTOMERS===\n");
    CLPrint(custList);
    CLDispose(custList);
    TLDispose(tarifList);
    free(custList);
    free(tarifList);
    return 0;
}
