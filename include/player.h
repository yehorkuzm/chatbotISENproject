#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    char name[50];
    double balance;
} Player;

Player create_player(const char *name, double initial_balance);
void print_player(const Player *p);

#endif
