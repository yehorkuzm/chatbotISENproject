#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    char name[50];
    double balance;
    int show_commands;  // 1 = show, 0 = hide
} Player;

Player create_player(const char *name, double initial_balance);
void print_player(const Player *p);

#endif
