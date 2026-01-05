#ifndef DATABASE_H
#define DATABASE_H

#include "player.h"

#define PLAYER_DB_DIR "db"
#define PLAYER_DB_FILE "players.txt"
#define RESPONSES_FILE "responses.txt"

// Save player to database
int save_player_to_db(const Player *p);

// List all players
void list_all_players();

// Load player from database by name
int load_player_from_db(const char *name, Player *player);

// Check if player exists in database
int player_exists(const char *name);

// Get full path for responses file
void get_responses_path(char *path, size_t size);


// Update player balance in database
int update_player_in_db(const Player *player);

// Save last played player name
void save_last_player(const char *name);

// Load last played player name (returns 1 if found and player exists, 0 otherwise)
int load_last_player(char *name, size_t size);

#endif