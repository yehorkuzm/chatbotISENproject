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
    double balance;

    do {
        printf("\n=== HOTEL SIMULATOR ===\n");
        printf("1. Create a new player\n");
        printf("2. View all players\n");
        printf("3. Start game\n");
        printf("4. Admin mode\n");
        printf("0. Quit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
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

            case 2:
                list_all_players();
                break;

            case 3:
                start_game();
                break;
            case 4:
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