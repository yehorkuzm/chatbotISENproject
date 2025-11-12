#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/player.h"
#include "../include/chatbot.h"
#include "../include/database.h"

void start_game() {
    char player_name[50];
    printf("\nEnter your player name to start the game: ");
    fgets(player_name, sizeof(player_name), stdin);
    player_name[strcspn(player_name, "\n")] = '\0';

    Player player;
    
    // Check if player exists in database
    if (player_exists(player_name)) {
        if (load_player_from_db(player_name, &player)) {
            printf("Welcome back, %s! Your balance is %.2f €\n", player.name, player.balance);
        } else {
            printf("Error loading player data.\n");
            return;
        }
    } else {
        printf("Player '%s' not found in database.\n", player_name);
        printf("Please create a player first from the main menu.\n");
        return;
    }
    
    // Get responses file path from database module
    char responses_path[256];
    get_responses_path(responses_path, sizeof(responses_path));
    
    Chatbot bot;
    if (!init_chatbot(&bot, responses_path)) {
        printf("Error: Failed to initialize chatbot. Make sure %s exists.\n", responses_path);
        return;
    }

    printf("\n=== WELCOME TO HOTEL GRAND ISEN ===\n");
    printf("Narrator: Welcome, %s! You've just arrived at the Hotel Grand ISEN.\n", player.name);
    printf("Narrator: You're living in room 204, but your savings are running low (%.2f €).\n", player.balance);
    printf("Narrator: You need to find work in the hotel to pay your rent and survive.\n");
    printf("Narrator: Talk to me about what you want to do. Type 'quit' to exit the game.\n\n");

    char input[256];
    int game_running = 1;

    while (game_running) {
        printf("%s: ", player.name);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("Narrator: Goodbye, %s! Come back soon.\n", player.name);
            game_running = 0;
        } else {
            process_input(&bot, &player, input);
        }

        if (player.balance <= 0) {
            printf("\nNarrator: Oh no! You've run out of money and can't afford your room anymore.\n");
            printf("Narrator: GAME OVER.\n");
            game_running = 0;
        }
    }

    cleanup_chatbot(&bot);
    
    // Save player progress before exiting
    printf("Saving your progress...\n");
    // Note: This will append to file. Consider updating existing player instead.
}