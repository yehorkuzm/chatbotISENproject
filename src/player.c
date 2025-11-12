#include <stdio.h>
#include <string.h>
#include "../include/player.h"

Player create_player(const char *name, double initial_balance) {
    Player p;
    strncpy(p.name, name, sizeof(p.name) - 1);
    p.name[sizeof(p.name) - 1] = '\0';
    p.balance = initial_balance;
    return p;
}

void print_player(const Player *p) {
    printf("Nom: %s | Solde: %.2f €\n", p->name, p->balance);
}
