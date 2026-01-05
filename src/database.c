#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../include/database.h"

static void ensure_db_directory_exists() {
    struct stat st = {0};
    char absolute_path[256];

    // Get the absolute path of the db directory
    _fullpath(absolute_path, PLAYER_DB_DIR, sizeof(absolute_path));
    printf("Checking or creating directory at: %s\n", absolute_path);

    if (stat(PLAYER_DB_DIR, &st) == -1) {
        mkdir(PLAYER_DB_DIR); // Create the "db" directory if it doesn't exist
        printf("Directory created at: %s\n", absolute_path);
    }
}

int save_player_to_db(const Player *p) {
    ensure_db_directory_exists();

    char path[256];
    snprintf(path, sizeof(path), "%s/%s", PLAYER_DB_DIR, PLAYER_DB_FILE);

    FILE *file = fopen(path, "a");
    if (!file) {
        perror("Error opening file");
        return 0;
    }
    fprintf(file, "%s %.2f %d\n", p->name, p->balance, p->show_commands);
    fclose(file);
    return 1;
}

void list_all_players() {
    ensure_db_directory_exists();

    char path[256];
    snprintf(path, sizeof(path), "%s/%s", PLAYER_DB_DIR, PLAYER_DB_FILE);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("No players found.\n");
        return;
    }

    char line[256];
    printf("=== All Players ===\n");
    while (fgets(line, sizeof(line), file)) {
        Player temp;
        temp.show_commands = 1;
        if (sscanf(line, "%49s %lf %d", temp.name, &temp.balance, &temp.show_commands) >= 2) {
            print_player(&temp);
        }
    }

    fclose(file);
}

int player_exists(const char *name) {
    ensure_db_directory_exists();

    char path[256];
    snprintf(path, sizeof(path), "%s/%s", PLAYER_DB_DIR, PLAYER_DB_FILE);

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0; // File doesn't exist, so player doesn't exist
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char temp_name[50];
        double temp_balance;
        if (sscanf(line, "%49s %lf", temp_name, &temp_balance) >= 2) {
            if (strcmp(temp_name, name) == 0) {
                fclose(file);
                return 1; // Player found
            }
        }
    }

    fclose(file);
    return 0; // Player not found
}

int load_player_from_db(const char *name, Player *player) {
    ensure_db_directory_exists();

    char path[256];
    snprintf(path, sizeof(path), "%s/%s", PLAYER_DB_DIR, PLAYER_DB_FILE);

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0; // File doesn't exist
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Player temp;
        temp.show_commands = 1;  // Default value

        // Try to parse with show_commands, falls back gracefully
        if (sscanf(line, "%49s %lf %d", temp.name, &temp.balance, &temp.show_commands) >= 2) {
            if (strcmp(temp.name, name) == 0) {
                *player = temp;
                fclose(file);
                return 1; // Player loaded successfully
            }
        }
    }

    fclose(file);
    return 0; // Player not found
}

void get_responses_path(char *path, size_t size) {
    ensure_db_directory_exists();
    snprintf(path, size, "%s/%s", PLAYER_DB_DIR, RESPONSES_FILE);
}

int update_player_in_db(const Player *player) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", PLAYER_DB_DIR, PLAYER_DB_FILE);

    // Read all players into memory
    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    Player players[100];
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && count < 100) {
        players[count].show_commands = 1;  // Default
        if (sscanf(line, "%49s %lf %d", players[count].name, &players[count].balance, &players[count].show_commands) >= 2) {
            count++;
        }
    }
    fclose(file);

    // Update the target player
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(players[i].name, player->name) == 0) {
            players[i].balance = player->balance;
            players[i].show_commands = player->show_commands;
            found = 1;
            break;
        }
    }

    if (!found) {
        return 0;
    }

    // Rewrite the file
    file = fopen(path, "w");
    if (!file) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %.2f %d\n", players[i].name, players[i].balance, players[i].show_commands);
    }
    fclose(file);

    return 1;
}

void save_last_player(const char *name) {
    ensure_db_directory_exists();

    char path[256];
    snprintf(path, sizeof(path), "%s/last_player.txt", PLAYER_DB_DIR);

    FILE *file = fopen(path, "w");
    if (file) {
        fprintf(file, "%s\n", name);
        fclose(file);
    }
}

int load_last_player(char *name, size_t size) {
    char path[256];
    snprintf(path, sizeof(path), "%s/last_player.txt", PLAYER_DB_DIR);

    FILE *file = fopen(path, "r");
    if (!file) {
        return 0;
    }

    if (fgets(name, size, file)) {
        name[strcspn(name, "\n")] = '\0';
        fclose(file);

        // Verify player still exists
        if (player_exists(name)) {
            return 1;
        }
    } else {
        fclose(file);
    }

    return 0;
}
