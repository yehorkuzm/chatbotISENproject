#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/database.h"
#include "../include/utils.h"
#include "../include/game.h"
#include "../include/admin.h"

int main() {
    int choice;
    char name[50];
    char last_player[50];
    int has_last_player;
    Player last_player_data;

    do {
        // Check for last player each time menu is shown
        has_last_player = load_last_player(last_player, sizeof(last_player));
        if (has_last_player) {
            load_player_from_db(last_player, &last_player_data);
        }

        printf("\n=== HOTEL SIMULATOR ===\n");
        if (has_last_player) {
            printf("1. Continue as %s (%.2f EUR)\n", last_player, last_player_data.balance);
        }
        printf("2. Create a new player\n");
        printf("3. View all players\n");
        printf("4. Start game\n");
        printf("5. Admin mode\n");
        printf("0. Quit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                if (has_last_player) {
                    continue_game(last_player);
                } else {
                    printf("Invalid choice.\n");
                }
                break;

            case 2:
                printf("Enter your name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                // Check if player already exists
                if (player_exists(name)) {
                    printf("Player '%s' already exists! Please choose a different name or start the game.\n", name);
                    break;
                }

                Player p = create_player(name, 200);
                if (save_player_to_db(&p))
                    printf("Player %s saved successfully!\n", p.name);
                else
                    printf("Error saving player.\n");
                break;

            case 3:
                list_all_players();
                break;

            case 4:
                start_game();
                break;

            case 5:
                admin_menu();
                break;

            case 0:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
