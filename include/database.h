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


#endif