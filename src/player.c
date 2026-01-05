#include <stdio.h>
#include <string.h>
#include "../include/player.h"

Player create_player(const char *name, double initial_balance) {
    Player p;
    strncpy(p.name, name, sizeof(p.name) - 1);
    p.name[sizeof(p.name) - 1] = '\0';
    p.balance = initial_balance;
    p.show_commands = 1;  // Show commands by default
    return p;
}

void print_player(const Player *p) {
    printf("Name: %s | Balance: %.2f EUR\n", p->name, p->balance);
}
